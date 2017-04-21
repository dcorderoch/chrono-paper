#include "devices/timer.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include "devices/pit.h"
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "threads/thread.h"
/* Add provided list structure to implement queue. */
#include <kernel/list.h> 
  
#if TIMER_FREQ < 19
#error 8254 timer requires TIMER_FREQ >= 19
#endif
#if TIMER_FREQ > 1000
#error TIMER_FREQ <= 1000 recommended
#endif

/* Queue of "sleeping" threads. */
struct list wait_list;

/* Number of timer/clock ticks since OS booted. */
static int64_t ticks;

/* No. of loops per timer/clock tick.
   Initialized by timer_calibrate(). */
static unsigned loops_per_tick;

static intr_handler_func timer_interrupt;
static bool too_many_loops (unsigned loops);
static void busy_wait (int64_t loops);
static void real_time_sleep (int64_t num, int32_t denom);
static void real_time_delay (int64_t num, int32_t denom);

/* Set timer to interrupt TIMER_FREQ times per second,
   and registers the corresponding interrupt. */
void
timer_init (void) 
{
  pit_configure_channel (0, 2, TIMER_FREQ);
  intr_register_ext (0x20, timer_interrupt, "8254 Timer");
  list_init (&wait_list);
}

/* Calibrate loops_per_tick, to implement brief delays. */
void
timer_calibrate (void) 
{
  unsigned high_bit, test_bit;

  ASSERT (intr_get_level () == INTR_ON);
  printf ("Calibrating timer...  ");

  /* Approximate loops_per_tick as the largest power-of-two
     that's still lower than one timer/clock tick. */
  loops_per_tick = 1u << 10;
  while (!too_many_loops (loops_per_tick << 1)) 
    {
      loops_per_tick <<= 1;
      ASSERT (loops_per_tick != 0);
    }

  /* Refine the next 8 bits of loops_per_tick. */
  high_bit = loops_per_tick;
  for (test_bit = high_bit >> 1; test_bit != high_bit >> 10; test_bit >>= 1)
    {
      if (!too_many_loops (high_bit | test_bit))
        loops_per_tick |= test_bit;
    }

  printf ("%'"PRIu64" loops/s.\n", (uint64_t) loops_per_tick * TIMER_FREQ);
}

/* Returns the timer/clock ticks since OS booted. */
int64_t
timer_ticks (void) 
{
  enum intr_level prev_level = intr_disable ();
  int64_t t = ticks;
  intr_set_level (prev_level);
  return t;
}

/* Returns the timer/clock ticks since THEN. */
int64_t
timer_elapsed (int64_t then) 
{
  return timer_ticks () - then;
}

/* Sleeps for about BEAT timer/clock ticks.
   Interrupts must be turned on. */
void
timer_sleep (int64_t beat) 
{
  struct thread *curr_thread;
  enum intr_level prev_level;

  ASSERT (intr_get_level () == INTR_ON); /* INTR_ON means interrupts are enabled */

  if (ticks <= 0) /* no process has taken the CPU, so no need to sleep. */
    return;

  prev_level = intr_disable ();

  /* Get current thread and set its wakeup ticks. */
  curr_thread = thread_current ();
  curr_thread->wakeup_ticks = timer_ticks () + beat;

  /* Insert current thread to ordered wait queue */
  list_insert_ordered (&wait_list, &curr_thread->elem,
                       thread_wakeup_ticks_less, NULL);
  thread_block ();

  intr_set_level (prev_level);
}

/* Sleeps for about MS milliseconds.
   Interrupts must be turned on. */
void
timer_msleep (int64_t ms) 
{
  real_time_sleep (ms, 1000);
}

/* Sleeps for about US microseconds.
   Interrupts must be turned on. */
void
timer_usleep (int64_t us) 
{
  real_time_sleep (us, 1000 * 1000);
}

/* Sleeps for about NS nanoseconds.
   Interrupts must be turned on. */
void
timer_nsleep (int64_t ns) 
{
  real_time_sleep (ns, 1000 * 1000 * 1000);
}

/* Sleeps for about MS microseconds
   Interrupts need not be turned on.

   "Busy waiting" wastes CPU cycles, and with
   interrupts off for the interval between timer/clock ticks or longer
   will cause timer/clock ticks to be lost. So, it's better to
   use timer_usleep() instead, if interrupts are enabled. */
void
timer_mdelay (int64_t ms) 
{
  real_time_delay (ms, 1000);
}

/* Sleeps for about US microseconds
   Interrupts need not be turned on.

   "Busy waiting" wastes CPU cycles, and with
   interrupts off for the interval between timer/clock ticks or longer
   will cause timer/clock ticks to be lost. So, it's better to
   use timer_usleep() instead, if interrupts are enabled. */
void
timer_udelay (int64_t us) 
{
  real_time_delay (us, 1000 * 1000);
}

/* Sleeps for about US microseconds
   Interrupts need not be turned on.

   "Busy waiting" wastes CPU cycles, and with
   interrupts off for the interval between timer/clock ticks or longer
   will cause timer/clock ticks to be lost. So, it's better to
   use timer_usleep() instead, if interrupts are enabled. */
void
timer_ndelay (int64_t ns) 
{
  real_time_delay (ns, 1000 * 1000 * 1000);
}

/* Prints time statistics. */
void
timer_print_stats (void) 
{
  printf ("timer: %"PRId64" ticks\n", timer_ticks ());
}

/* Timer interrupt handler. */
static void
timer_interrupt (struct intr_frame *args UNUSED)
{
  struct list_elem *list_element;
  struct thread *queue_tail;
  bool preempt = false;

  ticks++;
  thread_tick ();

  /* BSD4.4 scheduler. */
  if (thread_mlfqs)
    {
      thread_mlfqs_incr_recent_cpu ();
      if (ticks % TIMER_FREQ == 0)
        thread_mlfqs_refresh ();
      else if (ticks % 4 == 0)
        thread_mlfqs_update_priority (thread_current ());
    }

  /* Check and wake up waiting threads. */
  while (!list_empty (&wait_list))
    {
      list_element = list_front (&wait_list);
      queue_tail = list_entry (list_element, struct thread, elem);
      if ((queue_tail->wakeup_ticks) > ticks)
        break;
      list_remove (list_element);
      thread_unblock (queue_tail);
      preempt = true;
    }
  if (preempt)
    intr_yield_on_return ();
}

/* Returns true if LOOPS iterations waits for more than one timer
   tick, otherwise false. */
static bool
too_many_loops (unsigned loops) 
{
  /* Wait for a timer/clock tick. */
  int64_t start = ticks;
  while (ticks == start)
    barrier ();

  /* Run LOOPS loops. */
  start = ticks;
  busy_wait (loops);

  /* If the tick count changed, we iterated too long. */
  barrier ();
  return start != ticks;
}

/* Iterates through a simple loop ITERATIONS times,
   to implement brief delays.

   Marked NO_INLINE because code alignment can significantly
   affect timings, so that if this function was inlined
   differently in different places, results would be difficult
   to predict. */
static void NO_INLINE
busy_wait (int64_t iterations) 
{
  while (iterations-- > 0)
    barrier ();
}

/* Sleeps for about NUM/DENOM seconds. */
static void
real_time_sleep (int64_t num, int32_t denom) 
{
  /* Convert NUM/DENOM seconds into timer/clock ticks, rounding down.
     (NUM / DENOM) / (1 / TIMER_FREQ )  = NUM * TIMER_FREQ / DENOM ticks. 
  */
  int64_t ticks = num * TIMER_FREQ / denom;

  ASSERT (intr_get_level () == INTR_ON);
  if (ticks > 0)
    {
      /* Wait for at least one full timer/clock tick.  Use
         timer_sleep() to yield the CPU to other processes. */                
      timer_sleep (ticks); 
    }
  else 
    {
      /* Or "busy-wait" for more accurate timing. */
      real_time_delay (num, denom); 
    }
}

/* "Busy-wait" for about NUM/DENOM seconds. */
static void
real_time_delay (int64_t num, int32_t denom)
{
  /* Scale numerator and denominator down to avoid an overflow. */
  ASSERT (denom % 1000 == 0);
  busy_wait (loops_per_tick * num / 1000 * TIMER_FREQ / (denom / 1000)); 
}

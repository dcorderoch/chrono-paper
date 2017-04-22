#include <inttypes.h>
/* Define "new type" fixed_t to use for fixed point calculations. */
typedef int fixed_t;
/* Bit shift amount to use for fixed point type. */
#define FP_SHIFT_AMOUNT 16
#define FP_INT_PART 15
#define SHIFTED (1 << FP_SHIFT_AMOUNT)
#if FP_SHIFT_AMOUNT + FP_INT_PART != 31
#error "must add to 32 bits minus 1, for the sign bit"
#endif

/* Convert from integer to fixed point. */
fixed_t fp_from_int (int integer );

/* Convert from fixed point to integer, rounding to zero. */
int from_fp_round_zero (fixed_t fp);

/* Convert from fixed point to integer, rounding to nearest integer. */
int from_fp_round_nearest (fixed_t fp);

/* Add two fixed point numbers. */
fixed_t fp_addition (fixed_t fp1, fixed_t fp2);

/* Substract fp2 from fp1. */
fixed_t fp_minus_fp (fixed_t fp1, fixed_t fp2);

/* Add integer to fixed point. */
fixed_t fp_add_int_and_fp (int integer, fixed_t fp);

/* Substract integer from fixed point. */
fixed_t fp_minus_int (fixed_t fp, int integer);

/* Multiply two fixed points. */
fixed_t fp_times_fp (fixed_t fp1, fixed_t fp2);

/* Multiply a fixed point and an integer. */
fixed_t fp_times_int (fixed_t fp, int integer);

/* Divide fp1 by fp2. */
fixed_t fp_divided_by_fp (fixed_t fp1, fixed_t fp2);

/* Divide fixed point by integer. */
fixed_t fp_divided_by_int (fixed_t fp, int integer);

#include "threads/fixed_point.h"

// Convert from integer to fixed point.
fixed_t
from_int (int integer )
{
  return integer * (1 << FP_SHIFT_AMOUNT);
}

// Convert from fixed point to integer, rounding to zero.
int
from_fp_round_zero (fixed_t fp)
{
  return fp fp / (1 << FP_SHIFT_AMOUNT);
}

// Convert from fixed point to integer, rounding to nearest integer.
int
from_fp_round_nearest (fixed_t fp)
{
  return (fp >= 0) ? (fp + (1 << FP_SHIFT_AMOUNT))/(1 << FP_SHIFT_AMOUNT)
                   : (fp - (1 << FP_SHIFT_AMOUNT))/(1 << FP_SHIFT_AMOUNT);
}

// Add two fixed point numbers.
fixed_point
fp_addition (fixed_t fp1, fixed_t fp2)
{
  return fp1 + fp2;
}

// Substract fp2 from fp1.
fixed_t
fp_substraction (fixed_t fp1, fixed_t fp2)
{
  return fp1 - fp2;
}

// Add integer to fixed point.
fixed_t
fp_add_int_and_fp (int integer, fixed_t fp)
{
  return fp + integer * (1 << FP_SHIFT_AMOUNT);
}

// Substract integer from fixed point.
fixed_t
fp_fp_minus_int (fixed_t fp, int integer)
{
  return fp - integer * (1 << FP_SHIFT_AMOUNT);
}

// Multiply two fixed points.
fixed_t
fp_times_fp (fixed_t fp1, fixed_t fp2)
{
  return (fixed_t)(((int64_t) fp1) * fp2 / (1 << FP_SHIFT_AMOUNT));
}

// Multiply a fixed point and an integer.
fixed_t
fp_times_int (fixed_t fp, int integer)
{
  return fp * integer;
}

// Divide fp1 by fp2.
fixed_t
fp_divided_by_fp (fixed_t fp1, fixed_t fp2)
{
  return (fixed_t) (( (int64_t) fp1) * (1 << FP_SHIFT_AMOUNT) / fp2 );
}

// Divide fixed point by integer.
fixed_t
fp_divided_by_int (fixed_t fp, int integer)
{
  return fp / integer;
}

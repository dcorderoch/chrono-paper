#include "fixed_point.h" /* typedef fixed_t, define SHIFTED. */

/* Convert from integer to fixed point. */
fixed_t
fp_from_int (int integer )
{
  return integer * (SHIFTED);
}

/* Convert from fixed point to integer, rounding to zero. */
int
from_fp_round_zero (fixed_t fp)
{
  return fp / (SHIFTED);
}

/* Convert from fixed point to integer, rounding to nearest integer. */
int
from_fp_round_nearest (fixed_t fp)
{
  return (fp >= 0) ? (fp + (SHIFTED))/(SHIFTED)
                   : (fp - (SHIFTED))/(SHIFTED);
}

/* Add two fixed point numbers. */
fixed_t
fp_addition (fixed_t fp1, fixed_t fp2)
{
  return fp1 + fp2;
}

/* Substract fp2 from fp1. */
fixed_t
fp_minus_fp (fixed_t fp1, fixed_t fp2)
{
  return fp1 - fp2;
}

/* Add integer to fixed point. */
fixed_t
fp_add_int_and_fp (int integer, fixed_t fp)
{
  return fp + integer * (SHIFTED);
}

/* Substract integer from fixed point. */
fixed_t
fp_minus_int (fixed_t fp, int integer)
{
  return fp - integer * (SHIFTED);
}

/* Multiply two fixed points. */
fixed_t
fp_times_fp (fixed_t fp1, fixed_t fp2)
{
  return (fixed_t)(((int64_t) fp1) * fp2 / (SHIFTED));
}

/* Multiply a fixed point and an integer. */
fixed_t
fp_times_int (fixed_t fp, int integer)
{
  return fp * integer;
}

/* Divide fp1 by fp2. */
fixed_t
fp_divided_by_fp (fixed_t fp1, fixed_t fp2)
{
  return (fixed_t) (( (int64_t) fp1) * (SHIFTED) / fp2 );
}

/* Divide fixed point by integer. */
fixed_t
fp_divided_by_int (fixed_t fp, int integer)
{
  return fp / integer;
}

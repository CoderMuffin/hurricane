#ifndef TAYLOR_MATH_H
#define TAYLOR_MATH_H

#include <hurricane/fixed.hpp>
#include <math.h>

/* Trigonometric */
fixed sin(fixed x);
fixed cos(fixed x);
fixed tan(fixed x);

/* Inverse trigonometric */
fixed asin(fixed x);
fixed acos(fixed x);
fixed atan(fixed x);
fixed atan2(fixed y, fixed x);

/* Misc */
fixed sqrt(fixed x);
fixed fabs(fixed x);
fixed copysign(fixed x, fixed y);

#undef isnan
int isnan(fixed x);

#undef isfinite
int isfinite(fixed x);

#endif

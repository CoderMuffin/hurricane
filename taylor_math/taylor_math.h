#ifndef TAYLOR_MATH_H
#define TAYLOR_MATH_H

#include "../hurricane/include/hurricane/fixed.h"

#define M_PI 3.1415926535897932

/* Trigonometric */
fixed fx_sin(fixed x);
fixed fx_cos(fixed x);
fixed fx_tan(fixed x);

/* Inverse trigonometric */
fixed fx_asin(fixed x);
fixed fx_acos(fixed x);
fixed fx_atan(fixed x);
fixed fx_atan2(fixed y, fixed x);

/* Misc */
fixed fx_sqrt(fixed x);
fixed fx_abs(fixed x);
fixed fx_copysign(fixed x, fixed y);

int fx_isnan(fixed x);

int fx_isfinite(fixed x);

#endif

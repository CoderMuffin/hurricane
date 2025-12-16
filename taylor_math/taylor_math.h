#ifndef TAYLOR_MATH_H
#define TAYLOR_MATH_H

#include <math.h>

/* Trigonometric */
double sin(double x);
double cos(double x);
double tan(double x);

/* Inverse trigonometric */
double asin(double x);
double acos(double x);
double atan(double x);
double atan2(double y, double x);

/* Misc */
double sqrt(double x);
double fabs(double x);
double copysign(double x, double y);

#undef isnan
int isnan(double x);

#undef isfinite
int isfinite(double x);

#endif

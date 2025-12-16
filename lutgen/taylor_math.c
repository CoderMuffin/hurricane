#include <stdint.h>
#include "taylor_math.h"

/* ---------- helpers ---------- */

double fabs(double x) {
    return x < 0 ? -x : x;
}

/* Range reduction to [-pi, pi] */
static double reduce(double x)
{
    const double inv_2pi = 1.0 / (2.0 * M_PI);
    long k = (long)(x * inv_2pi);
    x -= k * 2.0 * M_PI;
    if (x > M_PI)  x -= 2.0 * M_PI;
    if (x < -M_PI) x += 2.0 * M_PI;
    return x;
}

/* ---------- sin / cos ---------- */

/* 9th-degree minimax-ish polynomial */
double sin(double x)
{
    x = reduce(x);

    double x2 = x * x;
    return x * (1.0
        - x2 * (1.0/6.0
        - x2 * (1.0/120.0
        - x2 * (1.0/5040.0
        - x2 * (1.0/362880.0)))));
}

double cos(double x)
{
    x = reduce(x);

    double x2 = x * x;
    return 1.0
        - x2 * (1.0/2.0
        - x2 * (1.0/24.0
        - x2 * (1.0/720.0
        - x2 * (1.0/40320.0))));
}

double tan(double x)
{
    double c = cos(x);
    if (c == 0.0)
        return (x > 0 ? 1e308 : -1e308); /* crude overflow */

    return sin(x) / c;
}

/* ---------- atan ---------- */

/* Polynomial valid on [0,1] */
static double atan_poly(double x)
{
    double x2 = x * x;
    return x * (1.0
        - x2 * (1.0/3.0
        - x2 * (1.0/5.0
        - x2 * (1.0/7.0
        - x2 * (1.0/9.0)))));
}

double atan(double x)
{
    if (x < 0.0)
        return -atan(-x);

    if (x > 1.0)
        return M_PI_2 - atan_poly(1.0 / x);

    return atan_poly(x);
}

double atan2(double y, double x)
{
    if (x > 0.0)
        return atan(y / x);

    if (x < 0.0) {
        if (y >= 0.0)
            return atan(y / x) + M_PI;
        else
            return atan(y / x) - M_PI;
    }

    if (y > 0.0) return  M_PI_2;
    if (y < 0.0) return -M_PI_2;
    return 0.0;
}

/* ---------- asin / acos ---------- */

double asin(double x)
{
    if (x >  1.0) return  M_PI_2;
    if (x < -1.0) return -M_PI_2;

    /* asin(x) = atan(x / sqrt(1 - x²)) */
    return atan(x / sqrt(1.0 - x * x));
}

double acos(double x)
{
    return M_PI_2 - asin(x);
}

/* ---------- sqrt ---------- */

/* Newton–Raphson */
double sqrt(double x)
{
    if (x <= 0.0)
        return 0.0;

    double r = x;
    for (int i = 0; i < 10; i++)
        r = 0.5 * (r + x / r);

    return r;
}

double copysign(double x, double y)
{
    union {
        double d;
        uint64_t u;
    } ux, uy;

    ux.d = x;
    uy.d = y;

    // Clear sign bit of x, copy sign bit from y
    ux.u = (ux.u & 0x7FFFFFFFFFFFFFFFULL) |
           (uy.u & 0x8000000000000000ULL);

    return ux.d;
}

inline int isnan(double x)
{
    union {
        double d;
        uint64_t u;
    } v = { x };

    /* exponent all 1s, mantissa non-zero */
    return ((v.u & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) &&
           ((v.u & 0x000FFFFFFFFFFFFFULL) != 0);
}

inline int isfinite(double x)
{
    union {
        double d;
        uint64_t u;
    } v = { x };

    /* exponent not all 1s */
    return (v.u & 0x7FF0000000000000ULL) != 0x7FF0000000000000ULL;
}

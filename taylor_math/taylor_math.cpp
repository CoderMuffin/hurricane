#include <stdint.h>
#include "taylor_math.h"

/* ---------- helpers ---------- */

fixed fabs(fixed x) {
    return x < 0 ? -x : x;
}

/* Range reduction to [-pi, pi] */
static fixed reduce(fixed x)
{
    const fixed inv_2pi = 1.0 / (2.0 * M_PI);
    long k = (long)(x * inv_2pi);
    x -= k * 2.0 * M_PI;
    if (x > M_PI)  x -= 2.0 * M_PI;
    if (x < -M_PI) x += 2.0 * M_PI;
    return x;
}

/* ---------- sin / cos ---------- */

/* 9th-degree minimax-ish polynomial */
fixed sin(fixed x)
{
    x = reduce(x);

    fixed x2 = x * x;
    return x * (1.0
        - x2 * (1.0/6.0
        - x2 * (1.0/120.0
        - x2 * (1.0/5040.0
        - x2 * (1.0/362880.0)))));
}

fixed cos(fixed x)
{
    x = reduce(x);

    fixed x2 = x * x;
    return 1.0
        - x2 * (1.0/2.0
        - x2 * (1.0/24.0
        - x2 * (1.0/720.0
        - x2 * (1.0/40320.0))));
}

fixed tan(fixed x)
{
    fixed c = cos(x);
    if (c == 0.0)
        return (x > 0 ? 1e308 : -1e308); /* crude overflow */

    return sin(x) / c;
}

/* ---------- atan ---------- */

/* Polynomial valid on [0,1] */
static fixed atan_poly(fixed x)
{
    fixed x2 = x * x;
    return x * (1.0
        - x2 * (1.0/3.0
        - x2 * (1.0/5.0
        - x2 * (1.0/7.0
        - x2 * (1.0/9.0)))));
}

fixed atan(fixed x)
{
    if (x < 0.0)
        return -atan(-x);

    if (x > 1.0)
        return M_PI_2 - atan_poly(1.0 / x);

    return atan_poly(x);
}

fixed atan2(fixed y, fixed x)
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

fixed asin(fixed x)
{
    if (x >  1.0) return  M_PI_2;
    if (x < -1.0) return -M_PI_2;

    /* asin(x) = atan(x / sqrt(1 - x²)) */
    return atan(x / sqrt(1.0 - x * x));
}

fixed acos(fixed x)
{
    return M_PI_2 - asin(x);
}

/* ---------- sqrt ---------- */

/* Newton–Raphson */
fixed sqrt(fixed x)
{
    if (x <= 0.0)
        return 0.0;

    fixed r = x;
    for (int i = 0; i < 10; i++)
        r = 0.5 * (r + x / r);

    return r;
}

fixed copysign(fixed x, fixed y)
{
    union {
        fixed d;
        uint64_t u;
    } ux, uy;

    ux.d = x;
    uy.d = y;

    // Clear sign bit of x, copy sign bit from y
    ux.u = (ux.u & 0x7FFFFFFFFFFFFFFFULL) |
           (uy.u & 0x8000000000000000ULL);

    return ux.d;
}

inline int isnan(fixed x)
{
    union {
        fixed d;
        uint64_t u;
    } v = { x };

    /* exponent all 1s, mantissa non-zero */
    return ((v.u & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) &&
           ((v.u & 0x000FFFFFFFFFFFFFULL) != 0);
}

inline int isfinite(fixed x)
{
    union {
        fixed d;
        uint64_t u;
    } v = { x };

    /* exponent not all 1s */
    return (v.u & 0x7FF0000000000000ULL) != 0x7FF0000000000000ULL;
}

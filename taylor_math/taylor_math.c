#include <stdint.h>
#include <hurricane/fixed.h>
#include "taylor_math.h"

/* ---------- helpers ---------- */

fixed fx_abs(fixed x) {
    return x < 0 ? -x : x;
}

/* Range reduction to [-pi, pi] */
static fixed reduce(fixed x)
{
    const fixed inv_2pi = tf(1.0 / (2.0 * M_PI));
    long k = ti(fm(x, inv_2pi));
    x -= fm(k, tf(2.0 * M_PI));
    if (x > tf(M_PI))  x -= tf(2.0 * M_PI);
    if (x < -tf(M_PI)) x += tf(2.0 * M_PI);
    return x;
}

/* ---------- sin / cos ---------- */

/* 9th-degree minimax-ish polynomial */
fixed fx_sin(fixed x)
{
    x = reduce(x);

    fixed x2 = fm(x, x);
    return fm(x, tf(1.0)
        - fm(x2, tf(1.0/6.0)
        - fm(x2, tf(1.0/120.0)
        - fm(x2, tf(1.0/5040.0)
        - fm(x2, tf(1.0/362880.0))))));
}

fixed fx_cos(fixed x)
{
    x = reduce(x);

    fixed x2 = fm(x, x);
    return tf(1.0)
        - fm(x2, tf(1.0/2.0)
        - fm(x2, tf(1.0/24.0)
        - fm(x2, tf(1.0/720.0)
        - fm(x2, tf(1.0/40320.0)))));
}

fixed fx_tan(fixed x)
{
    fixed c = fx_cos(x);
    if (c == tf(0.0))
        return (x > tf(0) ? tf(1000) : -tf(1000)); /* crude overflow */

    return fd(fx_sin(x), c);
}

/* ---------- atan ---------- */

/* Polynomial valid on [0,1] */
static fixed atan_poly(fixed x)
{
    fixed x2 = fm(x, x);
    return fm( x, tf(1.0)
        -  fm(x2, tf(1.0/3.0)
        -  fm(x2, tf(1.0/5.0)
        -  fm(x2, tf(1.0/7.0)
        -  fm(x2, tf(1.0/9.0))))));
}

fixed fx_atan(fixed x)
{
    if (x < tf(0.0))
        return -fx_atan(-x);

    if (x > tf(1.0))
        return tf(M_PI / 2) - atan_poly(fd(tf(1.0), x));

    return atan_poly(x);
}

fixed fx_atan2(fixed y, fixed x)
{
    if (x > 0.0)
        return fx_atan(fd(y, x));

    if (x < 0.0) {
        if (y >= 0.0)
            return fx_atan(fd(y, x)) + tf(M_PI);
        else
            return fx_atan(fd(y, x)) - tf(M_PI);
    }

    if (y > 0.0) return  tf(M_PI / 2);
    if (y < 0.0) return -tf(M_PI / 2);
    return tf(0.0);
}

/* ---------- asin / acos ---------- */

fixed fx_asin(fixed x)
{
    if (x >  tf(1.0)) return  tf(M_PI / 2);
    if (x < -tf(1.0)) return -tf(M_PI / 2);

    /* asin(x) = atan(x / sqrt(1 - x²)) */
    return fx_atan(fd(x, fx_sqrt(tf(1.0) - fm(x, x))));
}

fixed fx_acos(fixed x)
{
    return tf(M_PI/2) - fx_asin(x);
}

/* ---------- sqrt ---------- */

/* Newton–Raphson */
fixed fx_sqrt(fixed x)
{
    if (x <= 0.0)
        return tf(0.0);

    fixed r = x;
    for (int i = 0; i < 10; i++)
        r = fm(tf(0.5), r + fd(x, r));

    return r;
}

fixed fx_copysign(fixed x, fixed y)
{
    return y < 0 ? -fx_abs(x) : fx_abs(x);
}

inline int fx_isnan(fixed x)
{
    return 0;
}

inline int fx_isfinite(fixed x)
{
    return 1;
}

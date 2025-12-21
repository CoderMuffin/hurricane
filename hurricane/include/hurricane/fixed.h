#ifndef HC_FIXED_H
#define HC_FIXED_H

#include <stdint.h>

#define FIXED_FRACBITS 16

#define fixed int32_t
#define fixed_widen(x) ((int64_t)(x))
#define tf(value) ((fixed)((value) * (1 << FIXED_FRACBITS)))
#define ti(value) ((int)((value) / (1 << FIXED_FRACBITS)))
#define fm(a, b) ((fixed)((fixed_widen(a) * fixed_widen(b)) >> FIXED_FRACBITS))
#define fd(a, b) ((fixed)((fixed_widen(a) << FIXED_FRACBITS) / fixed_widen(b)))

#endif
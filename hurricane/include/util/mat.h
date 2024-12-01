#ifndef HC_MAT_H
#define HC_MAT_H

//macros
#include "../util/vec.h"

//types

//decls

//functions
void hc_mat4x4_identity(double out[4][4]);
void hc_mat4x4_print(const double mat[4][4]);
void hc_mat4x4_mul(double a[4][4], double b[4][4], double out[4][4]);
void hc_mat4x4_mul_vec(double m[4][4], double v[3], double out[3]);

#endif

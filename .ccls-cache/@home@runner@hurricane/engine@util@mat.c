#ifndef HC_MAT
#define HC_MAT
#include "vec.c"

void hc_mat4x4_mul(double a[4][4], double b[4][4], double out[4][4]) {
    for (short i = 0; i < 4; ++i) {
        for (short j = 0; j < 4; ++j) {
            out[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
                a[i][2] * b[2][j] + a[i][3] * b[3][j];
        }
    }
}

// double **hc_mat4x4_transpose(double m[4][4]) {
//     double out[4][4] = {
//         { m[0][0], m[1][0], m[2][0], m[3][0], },
//         { m[0][1], m[1][1], m[2][1], m[3][1], },
//         { m[0][2], m[1][2], m[2][2], m[3][2], },
//         { m[0][3], m[1][3], m[2][3], m[3][3] }
//     };
//     return out;
// }

void hc_mat4x4_mul_vec(double m[4][4], double v[3], double out[3]) {
    double a, b, c, w;
    
    a = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + m[3][0];
    b = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + m[3][1];
    c = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + m[3][2];
    w = v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + m[3][3];
    
    out[0] = a / w;
    out[1] = b / w;
    out[2] = c / w;
}

#endif
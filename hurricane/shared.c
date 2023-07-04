#ifndef HC_SHARED
#define HC_SHARED
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define HC_RENDER_SIZE_X 512
#define HC_RENDER_SIZE_Y 512
#define HC_RENDER_SIZE_STR_X STR(HC_RENDER_SIZE_X)
#define HC_RENDER_SIZE_STR_Y STR(HC_RENDER_SIZE_Y)
#define DEG2RAD (M_PI / 180)
#define VEC3(x, y, z) ((double[]){(x), (y), (z)})
#define QUAT(x, y, z) ((hc_quaternion[]){hc_quaternion_new_euler_zyx(VEC3((z), (y), (x)))})

int hc_render_bg[3] = {205, 20, 250};

void swap_int(int *a, int *b) {
  int temp = *b;
  *b = *a;
  *a = temp;
}
void swap_dbl(double *a, double *b) {
  double temp = *b;
  *b = *a;
  *a = temp;
}

double calculate_depth(double x0, double y0, double z0, double x1, double y1,
                       double z1, double x2, double y2, double z2, double px,
                       double py) {
  double w_denom = ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));

  if (w_denom == 0) {
    return 1000;
  }

  double w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / w_denom;
  double w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / w_denom;
  double w2 = 1 - w0 - w1;

  return (w0 * z0 + w1 * z1 + w2 * z2);
}

#endif

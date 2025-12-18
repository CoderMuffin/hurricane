#include <hurricane/shared.h>

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

#ifdef _WIN32
#include <windows.h>
#else
//#include <unistd.h>
#endif

void hc_sleep(long ms) {
#ifdef _WIN32
  Sleep(ms);
#else
  //usleep(ms*1000);
#endif
}

#ifndef HC_LOG_H
#define HC_LOG_H

extern const char *const hc_log_levels[4];


#define hc_log_handler(level, file, line, fmt, ...)                            \
  printf("[%s] %s:%d: " fmt "\n", hc_log_levels[level], file,                  \
         line __VA_OPT__(, ) __VA_ARGS__)
#define hc_log(fmt, ...)                                                       \
  hc_log_handler(0, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_info(fmt, ...)                                                      \
  hc_log_handler(1, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_warn(fmt, ...)                                                      \
  hc_log_handler(2, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_error(fmt, ...)                                                     \
  hc_log_handler(3, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)

#define hc_internal_log_handler(level, func, fmt, ...)                         \
  printf("[%s (\033[35mhurricane\033[0m)] <\033[36m%s\033[0m> " fmt "\n",        \
          hc_log_levels[level], func __VA_OPT__(, ) __VA_ARGS__)
#define hc_internal_log(fmt, ...)                                              \
  hc_internal_log_handler(0, __func__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_internal_info(fmt, ...)                                             \
  hc_internal_log_handler(1, __func__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_internal_warn(fmt, ...)                                             \
  hc_internal_log_handler(2, __func__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define hc_internal_error(fmt, ...)                                            \
  hc_internal_log_handler(3, __func__, fmt __VA_OPT__(, ) __VA_ARGS__)

#endif

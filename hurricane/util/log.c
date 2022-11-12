#ifndef HC_LOG
#define HC_LOG

const char* const hc_log_levels[4] = {
    "\033[92mLOG\033[0m",
    "\033[94mINFO\033[0m",
    "\033[33mWARN\033[0m",
    "\033[91mERROR\033[0m"
};

#define hc_log_handler(level, file, line, fmt, ...) printf("[%s] %s:%d: " fmt "\n", hc_log_levels[level], file, line  __VA_OPT__(,) __VA_ARGS__)
#define hc_log(fmt, ...) hc_log_handler(0, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define hc_info(fmt, ...) hc_log_handler(1, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define hc_warn(fmt, ...) hc_log_handler(2, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define hc_error(fmt, ...) hc_log_handler(3, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)

#endif

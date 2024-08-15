// Macro to use __cleanup__ attribute
#define AUTO_CLEANUP(var, cleanup_func) \
    var __attribute__((__cleanup__(cleanup_func)))
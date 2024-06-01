#pragma once

#define _concat(x, y) x ## y
#define concat(x, y) _concat(x, y)
#define macro_var(name) concat(name, __LINE__)
#define defer(start, end) for (      \
    int macro_var(_i_) = (start, 0); \
    !macro_var(_i_);                 \
    (macro_var(_i_) += 1), end)      \


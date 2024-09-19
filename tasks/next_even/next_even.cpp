#include "next_even.h"
#include <cassert>

int64_t NextEven(int64_t n) {
    assert(n != INT64_MAX);
    return n + 1 + (~n & 1);
}

#include "fibonacci.h"

enum FirstValues { F0 = static_cast<int64_t>(0), F1 = static_cast<int64_t>(1) };

int64_t CalculateNthFibonacci(int64_t n) {
    if (n == 0) {
        return F0;
    }

    int64_t prev_val = F0;
    int64_t cur_val = F1;

    while (--n) {
        cur_val = prev_val + cur_val;
        prev_val = cur_val - prev_val;
    }
    return cur_val;
}

#include "min_divisor.h"

const uint64_t MIN_NAT_NUMBER = 2;

int64_t MinDivisor(int64_t number) {
    for (int64_t d = MIN_NAT_NUMBER; d * d <= number; ++d) {
        if (number % d == 0) {
            return d;
        }
    }
    return number;
}

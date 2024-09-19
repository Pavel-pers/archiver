#include "reduce_fraction.h"
#include <numeric>
#include <algorithm>
#include <cassert>

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    assert(denominator != 0);
    if (denominator < 0) {
        numerator *= -1, denominator *= -1;
    }

    int64_t overall_coef = std::gcd(std::abs(numerator), std::abs(denominator));
    return (numerator / overall_coef + denominator / overall_coef);
}
#include "reduce_fraction.h"
#include <__numeric/gcd_lcm.h>

int64_t ReduceFraction(int64_t numerator, int64_t denominator) {
    return (numerator / std::gcd(numerator, denominator)) + (denominator / std::gcd(numerator, denominator));
}

#include "is_leap.h"
#include <cstdint>

const uint64_t PERIOD = 4;
const uint64_t EXCEPTION_PERIOD = 100;
const uint64_t ADDICTIONAL_PERIOD = 400;

bool IsLeap(int year) {
    return (year % PERIOD == 0 && year % EXCEPTION_PERIOD != 0) || (year % ADDICTIONAL_PERIOD == 0);
}

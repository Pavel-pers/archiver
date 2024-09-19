#include "is_leap.h"
#include <cstdint>

const uint32_t PERIOD = 4;
const uint32_t EXCEPTION_PERIOD = 100;
const uint32_t ADDICTIONAL_PERIOD = 400;

bool IsLeap(int year) {
    return (year % PERIOD == 0 && year % EXCEPTION_PERIOD != 0) || (year % ADDICTIONAL_PERIOD == 0);
}

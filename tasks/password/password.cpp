#include "password.h"

const char MIN_DIGIT = '0';
const char MAX_DIGIT = '9';
const char MIN_LOWER_CASE = 'a';
const char MAX_LOWER_CASE = 'z';
const char MIN_UPPER_CASE = 'A';
const char MAX_UPPER_CASE = 'Z';

const char MIN_ACCEPTABLE = 33;
const char MAX_ACCEPTABLE = 126;

const char MIN_STRING_LENGHT = 8;
const char MAX_STRING_LENGHT = 14;

bool ValidatePassword(const std::string& password) {
    if (MIN_STRING_LENGHT > password.size() || password.size() > MAX_STRING_LENGHT) {
        return false;
    }

    bool include_digits = false;
    bool include_lower_case = false;
    bool include_upper_case = false;
    bool include_undefined = false;

    for (char c : password) {
        if (MIN_ACCEPTABLE > c || MAX_ACCEPTABLE < c) {
            return false;
        }

        if (MIN_DIGIT <= c && c <= MAX_DIGIT) {
            include_digits = true;
        } else if (MIN_LOWER_CASE <= c && c <= MAX_LOWER_CASE) {
            include_lower_case = true;
        } else if (MIN_UPPER_CASE <= c && c <= MAX_UPPER_CASE) {
            include_upper_case = true;
        } else {
            include_undefined = true;
        }
    }

    return (include_digits + include_lower_case + include_upper_case + include_undefined) >= 3;
}

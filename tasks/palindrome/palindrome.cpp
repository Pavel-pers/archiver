#include "palindrome.h"

const size_t FIRST_STRING_INDEX = 0;

bool IsPalindrome(std::string const& str) {
    size_t left_index = FIRST_STRING_INDEX;
    size_t right_index = str.size() - 1;

    while (left_index < right_index) {
        if (str[left_index] == ' ') {
            ++left_index;
            continue;
        }
        if (str[right_index] == ' ') {
            --right_index;
            continue;
        }

        if (str[left_index] == str[right_index]) {
            ++left_index, --right_index;
        } else {
            return false;
        }
    }
    return true;
}

#include "provinces.h"

int64_t CountPassports(const std::vector<int>& provinces) {
    std::multiset<int64_t> ord;
    for (const int count : provinces) {
        ord.insert(static_cast<int64_t>(count));
    }

    int64_t min_cost = 0;
    while (ord.size() > 1) {
        int64_t new_size = *(ord.begin()) + *(++ord.begin());
        min_cost += new_size;
        ord.erase(ord.begin());
        ord.erase(ord.begin());
        ord.insert(new_size);
    }

    return min_cost;
}

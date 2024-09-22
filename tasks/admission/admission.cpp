#include "admission.h"
#include <algorithm>

bool operator<(const Date& lhs, const Date& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) < std::tie(rhs.year, rhs.month, rhs.day);
}

bool operator<(const Student& lhs, const Student& rhs) {
    return std::tie(lhs.birth_date, lhs.name) < std::tie(rhs.birth_date, rhs.name);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> ordered_applicants;
    for (const Applicant& applicant : applicants) {
        ordered_applicants.emplace_back(&applicant);
    }

    std::sort(ordered_applicants.begin(), ordered_applicants.end(), [](const Applicant* lhs, const Applicant* rhs) {
        return lhs->points > rhs->points || (lhs->points == rhs->points && lhs->student < rhs->student);
    });

    std::unordered_map<std::string, size_t> free_place_count;
    for (const auto& [name, cur_place_count] : universities) {
        free_place_count[name] = cur_place_count;
    }

    AdmissionTable result;

    for (const Applicant* applicant : ordered_applicants) {
        for (const std::string& university : applicant->wish_list) {
            if (free_place_count[university]) {
                result[university].push_back(&applicant->student);
                free_place_count[university]--;
                break;
            }
        }
    }

    return result;
}

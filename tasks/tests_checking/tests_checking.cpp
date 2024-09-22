#include "tests_checking.h"

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    size_t student_n = student_actions.size();
    if (student_n == 0) {
        return {};
    }

    std::vector<std::string> ordered_students(student_n);
    size_t topside_index = 0;
    size_t bottomside_index = student_n - 1;

    for (size_t act_i = student_n - 1;; --act_i) {
        if (student_actions[act_i].side == Side::Bottom) {
            ordered_students[bottomside_index] = student_actions[act_i].name;
            bottomside_index--;
        } else {
            ordered_students[topside_index] = student_actions[act_i].name;
            topside_index++;
        }

        if (act_i == 0) {
            break;
        }
    }

    std::vector<std::string> query_answers;
    for (size_t query : queries) {
        query_answers.push_back(ordered_students[query - 1]);
    }

    return query_answers;
}
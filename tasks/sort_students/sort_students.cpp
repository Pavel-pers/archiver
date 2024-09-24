#include "sort_students.h"
#include <algorithm>

void SortStudents(std::vector<Student>& students, SortKind const sort_kind) {
    std::sort(students.begin(), students.end(),
              sort_kind == SortKind::Name
                  ? [](Student left, Student right) {
                      return
                          std::tie(left.last_name, left.name,
                                     left.birth_date.year, left.birth_date.month, left.birth_date.day) <
                          std::tie(right.last_name, right.name,
                                     right.birth_date.year, right.birth_date.month, right.birth_date.day);
                  }
                  : [](Student left, Student right) {
                      return
                          std::tie(left.birth_date.year, left.birth_date.month, left.birth_date.day,
                                     left.last_name, left.name) <
                          std::tie(right.birth_date.year, right.birth_date.month, right.birth_date.day,
                                     right.last_name, right.name);
                  }
        );
}
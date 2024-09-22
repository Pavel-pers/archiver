#include "sort_students.h"

void SortStudents(std::vector<Student>& students, SortKind const sort_kind) {
    std::sort(students.begin(), students.end(),
              sort_kind == SortKind::Name
                  ? [](Student left, Student right) {
                      return
                          std::tuple(left.last_name, left.name,
                                     left.birth_date.year, left.birth_date.month, left.birth_date.day) <
                          std::tuple(right.last_name, right.name,
                                     right.birth_date.year, right.birth_date.month, right.birth_date.day);
                  }
                  : [](Student left, Student right) {
                      return
                          std::tuple(left.birth_date.year, left.birth_date.month, left.birth_date.day,
                                     left.last_name, left.name) <
                          std::tuple(right.birth_date.year, right.birth_date.month, right.birth_date.day,
                                     right.last_name, right.name);
                  }
        );
}
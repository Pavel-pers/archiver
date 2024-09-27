#include "unixpath.h"
#include <vector>
#include <cassert>

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    assert(!path.empty());

    std::vector<std::string_view> splited_path;
    if (path[0] != '/') {
        size_t dir_start = 1;
        for (size_t i = 1; i <= current_working_dir.size(); ++i) {
            if (i == current_working_dir.size() || current_working_dir[i] == '/') {
                if (i != dir_start) {
                    splited_path.push_back(current_working_dir.substr(dir_start, i - dir_start));
                }
                dir_start = i + 1;
            }
        }
    }

    size_t dir_start = 0;
    for (size_t i = 0; i <= path.size(); ++i) {
        if (i == path.size() || path[i] == '/') {
            std::string_view cur_name = path.substr(dir_start, i - dir_start);
            if (dir_start != i && cur_name != ".") {
                if (cur_name == "..") {
                    if (!splited_path.empty()) {
                        splited_path.pop_back();
                    }
                } else {
                    splited_path.push_back(cur_name);
                }
            }
            dir_start = i + 1;
        }
    }

    if (splited_path.empty()) {
        return "/";
    }

    std::string unixpath;
    for (const std::string_view name : splited_path) {
        unixpath += '/';
        unixpath += name;
    }

    return unixpath;
}

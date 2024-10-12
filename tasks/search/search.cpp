#include "search.h"
#include <algorithm>
#include <numeric>
#include <cctype>
#include <cmath>
#include <unordered_map>
#include <vector>

const double EPS = 1e-7;

class HashWord {
public:
    uint32_t operator()(const std::string_view s) const {
        uint32_t result_hash = 0;
        for (char c : s) {
            c = static_cast<char>(tolower(c));
            result_hash = (static_cast<uint64_t>(BASE) * static_cast<uint64_t>(result_hash) + (c - 'a')) % MOD;
        }
        return result_hash;
    }

private:
    static const uint32_t MOD = 1e9 + 7;
    static const uint32_t BASE = 31;
};

class CompareWord {
public:
    bool operator()(const std::string_view lhs, const std::string_view rhs) const {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        for (int i = 0; i < lhs.size(); i++) {
            if (tolower(lhs[i]) != tolower(rhs[i])) {
                return false;
            }
        }
        return true;
    }
};

size_t GetWordLen(std::string_view pref) {
    for (int d = 0; d < pref.size(); d++) {
        if (!isalpha(pref[d])) {
            return d;
        }
    }
    return pref.size();
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    if (text.empty() || query.empty()) {
        return {};
    }

    std::unordered_map<std::string_view, std::vector<std::pair<size_t, size_t>>, HashWord, CompareWord>
        word_concurrence;
    for (size_t l = 0, r = 0; r <= query.size(); r++) {
        if (r == query.size() || !isalpha(query[r])) {
            if (l != r) {
                word_concurrence.insert({query.substr(l, r - l), {}});
            }
            l = r + 1;
        }
    }

    std::vector<std::string_view> splited_str;
    for (size_t char_ind = 0, line_start = 0; char_ind <= text.size(); char_ind++) {
        if (char_ind == text.size() || text[char_ind] == '\n') {
            splited_str.push_back(text.substr(line_start, char_ind - line_start));
            line_start = char_ind + 1;
        }
    }

    std::vector<size_t> str_word_count(splited_str.size());
    for (size_t str_ind = 0; str_ind < splited_str.size(); str_ind++) {
        std::string_view line = splited_str[str_ind];
        for (size_t char_ind = 0; char_ind < line.size(); char_ind++) {
            size_t word_len = GetWordLen(line.substr(char_ind));
            if (word_len > 0) {
                str_word_count[str_ind]++;
                std::string_view word = line.substr(char_ind, word_len);
                if (word_concurrence.count(word)) {
                    auto& lines_indexes = word_concurrence[word];
                    if (lines_indexes.empty() || lines_indexes.back().first != str_ind) {
                        lines_indexes.push_back({str_ind, 0});
                    }
                    lines_indexes.back().second++;
                }
            }
            char_ind += word_len;
        }
    }

    std::vector<double> tf_idf(splited_str.size());
    for (auto const& [word, con] : word_concurrence) {
        if (con.empty()) {
            continue;
        }

        double idf = log(static_cast<double>(str_word_count.size()) / static_cast<double>(con.size()));
        for (auto [line_i, word_c] : con) {
            double tf = static_cast<double>(word_c) / static_cast<double>(str_word_count[line_i]);
            tf_idf[line_i] += tf * idf;
        }
    }

    std::vector<size_t> ordered_indx(splited_str.size());
    std::iota(ordered_indx.begin(), ordered_indx.end(), 0);
    std::sort(ordered_indx.begin(), ordered_indx.end(),
              [&tf_idf](size_t lhs, size_t rhs) { return tf_idf[lhs] > tf_idf[rhs]; });

    std::vector<std::string_view> result(results_count);
    for (size_t i = 0; i < results_count; i++) {
        if (tf_idf[ordered_indx[i]] < EPS) {
            break;
        }
        result[i] = splited_str[ordered_indx[i]];
    }

    return result;
}
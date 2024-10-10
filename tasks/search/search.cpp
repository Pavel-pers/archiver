#include "search.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <unordered_map>
#include <vector>

class WordHash {
public:
    __uint32_t operator()(const std::string_view s) const {
        __uint32_t hash = 0;
        for (char i : s) {
            hash = (static_cast<__uint64_t>(hash) * BASE + (tolower(i) - 'a' + 1)) % MOD;
        }
        return hash;
    }

private:
    static const __uint32_t BASE = 31;
    static const __uint32_t MOD = 1e9 + 7;
};

const double EPS = 1e-7;

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<std::string_view, std::vector<std::pair<size_t, size_t>>, WordHash> matches_str;
    for (size_t start_indx = 0; start_indx < query.size(); ++start_indx) {
        for (size_t len = 0;; ++len) {
            if (len + start_indx == query.size() || !std::isalpha(query[start_indx + len])) {
                if (len) {
                    matches_str.insert({query.substr(start_indx, len), {}});
                }
                start_indx += len;
                break;
            }
        }
    }

    std::vector<std::pair<double, std::string_view>> ordered_str;
    std::vector<size_t> count_of_words;

    for (size_t start_indx = 0, start_str = 0, cur_str = 0, word_count = 0; start_indx < text.size(); ++start_indx) {
        for (int len = 0;; len++) {
            if (start_indx + len == text.size() || !std::isalpha(text[start_indx + len])) {
                if (len) {
                    word_count++;
                    std::string_view cur_w = text.substr(start_indx, len);
                    if (matches_str.count(cur_w)) {
                        auto& matches = matches_str[cur_w];
                        if ((matches.empty() || matches.back().first != cur_str)) {
                            matches.push_back({cur_str, 0});
                        }
                        matches.back().second++;
                    }
                }

                start_indx += len;
                break;
            }
        }
        if (start_indx == text.size() || text[start_indx] == '\n') {
            ordered_str.push_back({0, text.substr(start_str, start_indx - start_str)});
            count_of_words.push_back(word_count);

            start_str = start_indx + 1;
            word_count = 0;
            cur_str++;
        }
    }

    for (auto const& [word, matches] : matches_str) {
        if (matches.empty()) {
            continue;
        }

        double cur_idf = std::log(static_cast<double>(ordered_str.size()) / static_cast<double>(matches.size()));
        for (auto [str_indx, cur_word_cnt] : matches) {
            double cur_tf = static_cast<double>(cur_word_cnt) / static_cast<double>(count_of_words[str_indx]);
            ordered_str[str_indx].first = std::max(ordered_str[str_indx].first, cur_idf * cur_tf);
        }
    }

    sort(ordered_str.rbegin(), ordered_str.rend(),
         [](std::pair<double, std::string_view> const& lhs, std::pair<double, std::string_view> const& rhs) {
             return rhs.first - lhs.first > EPS;
         });

    std::vector<std::string_view> top_k;
    for (auto [tf_idf, str] : ordered_str) {
        if (tf_idf < EPS || top_k.size() == results_count) {
            break;
        }
        top_k.push_back(str);
    }
    return top_k;
}
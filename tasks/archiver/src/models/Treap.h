#include <algorithm>
#include <vector>
#include <stdexcept>
#include <queue>
#include <cstdint>
#include "../utility.h"

class TreapNode {
public:
    explicit TreapNode(utility_types::PaddedByte byte);

    explicit TreapNode(TreapNode *left_child, TreapNode *right_child);

    // there is no distructor becouse of supposing that root of all the Nodes belongs Treap which has distructor
    bool IsLeaf() const;

    utility_types::PaddedByte GetValue() const;

    void SetValue(utility_types::PaddedByte new_value);

    TreapNode *GetChild(bool bit) const; // returns nulltr in case of not having child
    void SetChild(bool bit, TreapNode *child);

private:
    utility_types::PaddedByte value_;
    TreapNode *ej_[2];
};


class TreapAutomata {
public:
    explicit TreapAutomata(TreapNode *root);

    bool Feed(uint16_t chank,
              std::queue<utility_types::PaddedByte> &buffer); // puts to queue values which have found, returns false if path is invalid
private:
    TreapNode *const start_point_;
    TreapNode *cur_point_;

    bool InTerminalPoint() const;
};

class Treap {
public:
    explicit Treap(TreapNode *root);

    explicit Treap(
            std::vector<std::pair<utility_types::VariableLenghCode, utility_types::PaddedByte>> code_to_word_mapping); // build treap by table {{word, word_len}, value}
    ~Treap();

    Treap(const Treap &) = delete;

    void operator=(const Treap &) = delete;

    void AddWord(utility_types::VariableLenghCode key, utility_types::PaddedByte value);

    utility_types::ByteMappingTable GetMappingTable() const;

    TreapAutomata BuildAutomata();

private:
    TreapNode *const root_;
};


namespace treap_utility {
    const bool LEFT_NODE_CHILD = false;
    const bool RIGHT_NODE_CHILD = true;

    void DeleteSubtreap(TreapNode *root_of_subtreap);

    void
    AddWordToSubtreap(TreapNode *root_of_subtreap, const utility_types::VariableLenghCode code,
                      utility_types::PaddedByte value);

    void FillTableOnSubtreap(TreapNode *root_of_subtreap, utility_types::ByteMappingTable &table,
                        utility_types::VariableLenghCode scrached_code);
}

namespace treap_exceptions {
    class InvalidPathException : std::exception {
        const char *what() const noexcept override;
    };
}
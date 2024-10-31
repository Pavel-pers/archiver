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
    TreapAutomata(TreapNode *root, std::vector<utility_types::PaddedByte> pending_bytes); /*
    connection with automata should close after receiving pending byte */

    bool Feed(uint16_t chank, std::queue<utility_types::PaddedByte> &buffer); /*
    puts to queue values which have found, returns false if path is invalid */

    explicit operator bool() const; /*
    returns true when found pending byte */

private:
    TreapNode *const start_point_;
    TreapNode *cur_point_;
    const std::vector<utility_types::PaddedByte> pending_bytes_;
    bool found_peding_byte_;

    bool InTerminalPoint() const;
};

class Treap {
public:
    explicit Treap();

    explicit Treap(TreapNode *root);

    explicit Treap(
            std::vector<std::pair<utility_types::VariableLenghCode, utility_types::PaddedByte>> code_to_word_mapping); // build treap by table {{word, word_len}, value}
    ~Treap();

    Treap(const Treap &) = delete;

    void operator=(const Treap &) = delete;

    void AddWord(utility_types::VariableLenghCode key, utility_types::PaddedByte value);

    utility_types::ByteMappingTable GetMappingTable() const;

    TreapAutomata BuildAutomata(std::vector<utility_types::PaddedByte> last_byte_pending);

private:
    TreapNode *const root_;
    bool found_pending_byte_;
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

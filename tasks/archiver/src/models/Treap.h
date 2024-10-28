#include <cstdint>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <array>
#include <queue>
#include <cstdint>
#include <limits>

class TreapNode {
public:
    explicit TreapNode(uint8_t byte);

    explicit TreapNode(TreapNode *left_child, TreapNode *right_child);

    // there is no distructor becouse of supposing that root of all the Nodes belongs Treap which has distructor
    bool IsLeaf() const;

    uint8_t GetValue() const;

    void SetValue(uint8_t new_value);

    TreapNode *GetChild(bool bit) const; // returns nulltr in case of not having child
    void SetChild(bool bit, TreapNode *child);

private:
    uint8_t value_;
    TreapNode *ej_[2];
};


class TreapAutomata {
public:
    explicit TreapAutomata(TreapNode *root);

    bool Feed(uint16_t chank,
              std::queue<uint8_t> &buffer); // puts to queue values which have found, returns false if path is invalid
private:
    TreapNode *const start_point_;
    TreapNode *cur_point_;

    bool InTerminalPoint() const;
};

class Treap {
public:
    explicit Treap(TreapNode *root);

    Treap(std::vector<std::pair<std::pair<uint16_t, uint8_t>, uint8_t>> code_to_word_mapping); // build treap by table {{word, word_len}, value}
    ~Treap();

    Treap(const Treap &) = delete;

    void operator=(const Treap &) = delete;

    void AddWord(uint16_t word, size_t word_size, uint8_t value);

    std::array<std::pair<uint16_t, uint8_t>, std::numeric_limits<uint8_t>::max()> GetMappingTable() const;

    TreapAutomata buildAutomata();

private:
    TreapNode *const root_;
};


namespace treap_utility {
    const bool LEFT_NODE_CHILD = false;
    const bool RIGHT_NODE_CHILD = true;

    using MappingTable = std::array<std::pair<uint16_t, uint8_t>, std::numeric_limits<std::uint8_t>::max()>;

    void DeleteSubtreap(TreapNode *root_of_subtreap);

    void AddWordToSubtreap(TreapNode *root_of_subtreap, uint16_t word, size_t word_size, uint8_t value);

    void
    FillTableOnSubtreap(TreapNode *root_of_subtreap, MappingTable &table,
                        uint16_t scratched_word, uint8_t scratched_word_size);
}

namespace treap_exceptions {
    class InvalidPathException : std::exception {
        const char *what() const noexcept;
    };
}
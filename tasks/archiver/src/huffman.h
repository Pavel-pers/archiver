#include "models/Treap.h"
#include <vector>

namespace huffman{
    struct PriorityNode {
        size_t total_count;
        TreapNode* node;
        PriorityNode(size_t total_count, TreapNode* node): total_count(total_count), node(node) {};
    };

    bool CanonicalCompare(utility_types::VariableLenghCode lhs, utility_types::VariableLenghCode rhs);
    utility_types::VariableLenghCode CanonicalIncrement(utility_types::VariableLenghCode);
    bool operator >(PriorityNode lhs, PriorityNode rhs);

    Treap BuildTreapByFrequancy(const utility_types::ByteFrequancy &frequancy);

    void RefactorToCanonical(utility_types::ByteMappingTable& mapping_table);

    std::vector<size_t>GetIndexOrderByCanonical(const utility_types::ByteMappingTable& map_table);
}
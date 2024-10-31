#include "models/Treap.h"
#include <vector>

namespace huffman {
    namespace canonical_operations {
        bool CanonicalCompare(utility_types::VariableLenghCode lhs, utility_types::VariableLenghCode rhs);

        void CanonicalIncrement(utility_types::VariableLenghCode &);

        void CanonicalWidening(utility_types::VariableLenghCode &);
    }

    struct PriorityNode {
        size_t total_count;
        TreapNode *node;

        PriorityNode(size_t total_count, TreapNode *node) : total_count(total_count), node(node) {};
    };
    bool operator>(PriorityNode lhs, PriorityNode rhs);

    Treap BuildTreapByFrequancy(const utility_types::ByteFrequancy &frequancy);

    void RefactorToCanonical(utility_types::ByteMappingTable &mapping_table);

    void RestoreTreapByInfo(
            const utility_types::MappingTableInfo &info,
            const std::vector<utility_types::PaddedByte> &canonical_ordered_bytes,
            Treap& restore_to);

    std::vector<utility_types::PaddedByte> GetIndexOrderByCanonical(const utility_types::ByteMappingTable &map_table);
}
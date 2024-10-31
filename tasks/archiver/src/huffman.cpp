#include "huffman.h"
#include <queue>

using utility_types::ByteFrequancy, utility_types::ByteMappingTable, utility_types::CodeLengh, utility_types::BitCode, utility_types::PaddedByte;
using utility_types::VariableLenghCode;
using utility_types::MappingTableInfo;

namespace huffman {
    bool operator>(PriorityNode lhs, PriorityNode rhs) {
        return lhs.total_count > rhs.total_count ||
               (lhs.total_count == rhs.total_count && lhs.node->GetValue() > lhs.node->GetValue());
    }

    Treap BuildTreapByFrequancy(const ByteFrequancy &frequancy) {
        std::priority_queue<PriorityNode, std::vector<PriorityNode>, std::greater<PriorityNode>> node_queue;
        for (PaddedByte byte = 0; byte < SpecialSymbols::ALP_POWER; byte++) {
            node_queue.push(PriorityNode(frequancy[byte], new TreapNode(byte)));
        }

        while (node_queue.size() > 1) {
            size_t summary_count = 0;
            TreapNode *first_node = node_queue.top().node;
            summary_count += node_queue.top().total_count;
            node_queue.pop();
            TreapNode *second_node = node_queue.top().node;
            summary_count += node_queue.top().total_count;
            node_queue.pop();

            node_queue.push(PriorityNode(summary_count, new TreapNode(first_node, second_node)));
        }

        return Treap(node_queue.top().node);
    }

    bool CanonicalCompare(utility_types::VariableLenghCode lhs, utility_types::VariableLenghCode rhs) {
        return std::tie(lhs.lengh, lhs.code) < std::tie(rhs.lengh, rhs.code);
    }

    VariableLenghCode CanonicalIncrement(VariableLenghCode x) {
        for (size_t bi = x.lengh - 1;; bi--) {
            x.code ^= (1 << bi);
            if (x.code & (1 << bi)) {
                break;
            }
        }
        return x;
    };

    void RefactorToCanonical(ByteMappingTable &mapping_table) {
        std::vector<size_t> ord_code_indexes = GetIndexOrderByCanonical(mapping_table);

        VariableLenghCode canonical_val(0, 0);
        for (size_t i: ord_code_indexes) {
            while (canonical_val.lengh < mapping_table[i].lengh) {
                canonical_val.lengh++;
            }

            mapping_table[i] = canonical_val;
            canonical_val = CanonicalIncrement(canonical_val);
        }
    }

    std::vector<size_t> GetIndexOrderByCanonical(const ByteMappingTable &map_table) {
        std::vector<size_t> ordered_indexes;
        for (size_t i = 0; i < map_table.size(); i++) {
            if (map_table[i].lengh > 0) { // skip non set values
                ordered_indexes.push_back(i);
            }
        }

        std::sort(ordered_indexes.begin(), ordered_indexes.end(), [&map_table](size_t lhs, size_t rhs) {
            return CanonicalCompare(map_table[lhs], map_table[rhs]);
        });

        return ordered_indexes;
    }
}
#include "Treap.h"

TreapNode::TreapNode(uint8_t byte) : value_(byte), ej_{nullptr, nullptr} {}

TreapNode::TreapNode(TreapNode *left_child, TreapNode *right_child) : value_(
        std::min(left_child->value_, right_child->value_)), ej_{left_child, right_child} {}

uint8_t TreapNode::GetValue() const {
    return value_;
}

TreapNode *TreapNode::GetChild(bool bit) const {
    return ej_[bit];
}

bool TreapNode::IsLeaf() const {
    return !(ej_[treap_utility::LEFT_NODE_CHILD] || ej_[treap_utility::RIGHT_NODE_CHILD]);
}

void TreapNode::SetValue(uint8_t value) {
    value_ = value;
}

void TreapNode::SetChild(bool bit, TreapNode *child) {
    if (ej_[bit] != nullptr) {
        std::logic_error("memory is leaking");
    }
    ej_[bit] = child;
}


TreapAutomata::TreapAutomata(TreapNode *root): start_point_(root), cur_point_(root) {}

bool TreapAutomata::InTerminalPoint() const {
    return cur_point_->IsLeaf();
}

bool TreapAutomata::Feed(uint16_t chank, std::queue<uint8_t>& buffer) {
    for (uint8_t i = sizeof(uint16_t) * CHAR_BIT; i--;) {
        if (cur_point_->GetChild(chank & 1) == nullptr) {
            return false;
        }
        cur_point_ = cur_point_->GetChild(chank & 1);
        chank >>= 1;
        if (InTerminalPoint()) {
            uint8_t found_value = cur_point_->GetValue();
            cur_point_ = start_point_;
            buffer.push(found_value);
        }
    }
    return true;
}


Treap::Treap(TreapNode *root) : root_(root) {}

Treap::Treap(std::vector<std::pair<std::pair<uint16_t, uint8_t>, uint8_t>> code_to_word_mapping) :
    root_(new TreapNode(std::numeric_limits<uint8_t>::max())) {
    for (auto [word_info, value]: code_to_word_mapping) {
        auto [word, word_size] = word_info;
        AddWord(word, word_size, value);
    }
}

Treap::~Treap() {
    treap_utility::DeleteSubtreap(root_);
}

void Treap::AddWord(uint16_t word, size_t word_size, uint8_t value) {
    treap_utility::AddWordToSubtreap(root_, word, word_size, value);
}

treap_utility::MappingTable Treap::GetMappingTable() const {
    treap_utility::MappingTable mapping_table;
    mapping_table.fill({0, 0});
    treap_utility::FillTableOnSubtreap(root_, mapping_table, 0, 0);
    return mapping_table;
}

TreapAutomata Treap::buildAutomata() {
    return TreapAutomata(root_);
}


namespace treap_utility {
    void DeleteSubtreap(TreapNode *root_of_subtreap) {
        if (root_of_subtreap->GetChild(LEFT_NODE_CHILD) != nullptr) {
            DeleteSubtreap(root_of_subtreap->GetChild(LEFT_NODE_CHILD));
        }
        if (root_of_subtreap->GetChild(RIGHT_NODE_CHILD) != nullptr) {
            DeleteSubtreap(root_of_subtreap->GetChild(RIGHT_NODE_CHILD));
        }
    }

    void AddWordToSubtreap(TreapNode *root_of_subtreap, uint16_t word, size_t word_size, uint8_t value) {
        root_of_subtreap->SetValue(std::min(root_of_subtreap->GetValue(), value));

        if (!root_of_subtreap->GetChild(word & 1)) {
            root_of_subtreap->SetChild(word & 1, new TreapNode(value));
        }

        if (word_size > 1) {
            AddWordToSubtreap(root_of_subtreap->GetChild(word & 1), word >> 1, word_size - 1, value);
        }
    }

    void
    FillTableOnSubtreap(TreapNode *root_of_subtreap, MappingTable &table,
                        uint16_t scratched_word, uint8_t scratched_word_size) {
        if (root_of_subtreap->IsLeaf()) {
            table[root_of_subtreap->GetValue()] = std::make_pair(scratched_word, scratched_word_size);
            return;
        }

        if (root_of_subtreap->GetChild(LEFT_NODE_CHILD)) {
            FillTableOnSubtreap(root_of_subtreap->GetChild(LEFT_NODE_CHILD), table,
                                scratched_word, scratched_word_size + 1);
        }
        if (root_of_subtreap->GetChild(RIGHT_NODE_CHILD)) {
            FillTableOnSubtreap(root_of_subtreap->GetChild(RIGHT_NODE_CHILD), table,
                                scratched_word | (1 << scratched_word_size), scratched_word_size + 1);
        }
    }
}

const char *treap_exceptions::InvalidPathException::what() const noexcept {
    return "Invalid bit given, couldnt get this node";
}

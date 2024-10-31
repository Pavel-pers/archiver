#include "Treap.h"

using utility_types::ByteFrequancy, utility_types::ByteMappingTable, utility_types::CodeLengh, utility_types::BitCode, utility_types::PaddedByte;
using utility_types::VariableLenghCode;
using utility_types::MappingTableInfo;


TreapNode::TreapNode(PaddedByte byte) : value_(byte), ej_{nullptr, nullptr} {}

TreapNode::TreapNode(TreapNode *left_child, TreapNode *right_child) :
        value_(std::min(left_child->value_, right_child->value_)), ej_{left_child, right_child} {}

PaddedByte TreapNode::GetValue() const {
    return value_;
}

TreapNode *TreapNode::GetChild(bool bit) const {
    return ej_[bit];
}

bool TreapNode::IsLeaf() const {
    return !(ej_[treap_utility::LEFT_NODE_CHILD] || ej_[treap_utility::RIGHT_NODE_CHILD]);
}

void TreapNode::SetValue(PaddedByte value) {
    value_ = value;
}

void TreapNode::SetChild(bool bit, TreapNode *child) {
    if (ej_[bit] != nullptr) {
        std::logic_error("memory is leaking");
    }
    ej_[bit] = child;
}


TreapAutomata::TreapAutomata(TreapNode *root) : start_point_(root), cur_point_(root) {}

bool TreapAutomata::InTerminalPoint() const {
    return cur_point_->IsLeaf();
}

bool TreapAutomata::Feed(uint16_t chank, std::queue<PaddedByte> &buffer) {
    for (uint8_t i = sizeof(uint16_t) * CHAR_BIT; i--;) {
        if (cur_point_->GetChild(chank & 1) == nullptr) {
            return false;
        }
        cur_point_ = cur_point_->GetChild(chank & 1);
        chank >>= 1;
        if (InTerminalPoint()) {
            PaddedByte found_value = cur_point_->GetValue();
            cur_point_ = start_point_;
            buffer.push(found_value);
        }
    }
    return true;
}


Treap::Treap(TreapNode *root) : root_(root) {}

Treap::Treap(std::vector<std::pair<VariableLenghCode, PaddedByte>> code_to_word_mapping) :
        root_(new TreapNode(std::numeric_limits<PaddedByte>::max())) {
    for (auto [code, value]: code_to_word_mapping) {
        AddWord(code, value);
    }
}

Treap::~Treap() {
    treap_utility::DeleteSubtreap(root_);
}

void Treap::AddWord(VariableLenghCode code, PaddedByte value) {
    treap_utility::AddWordToSubtreap(root_, code, value);
}

ByteMappingTable Treap::GetMappingTable() const {
    ByteMappingTable mapping_table;
    mapping_table.fill(VariableLenghCode(0, 0));
    treap_utility::FillTableOnSubtreap(root_, mapping_table, VariableLenghCode(0, 0));
    return mapping_table;
}

TreapAutomata Treap::BuildAutomata() {
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

    void AddWordToSubtreap(TreapNode *root_of_subtreap, VariableLenghCode code, PaddedByte value) {
        root_of_subtreap->SetValue(std::min(root_of_subtreap->GetValue(), value));

        if (!root_of_subtreap->GetChild(code.code & 1)) {
            root_of_subtreap->SetChild(code.code & 1, new TreapNode(value));
        }

        if (code.lengh > 1) {
            VariableLenghCode next_code(code.code >> 1, code.lengh - 1); // TODO delete this shit
            AddWordToSubtreap(root_of_subtreap->GetChild(code.code & 1), next_code, value);
        }
    }

    void
    FillTableOnSubtreap(TreapNode *root_of_subtreap, ByteMappingTable &table, VariableLenghCode scrached_word) {
        if (root_of_subtreap->IsLeaf()) {
            table[root_of_subtreap->GetValue()] = scrached_word;
            return;
        }

        if (root_of_subtreap->GetChild(LEFT_NODE_CHILD)) {
            VariableLenghCode next_code(scrached_word.code, scrached_word.lengh + 1);
            FillTableOnSubtreap(root_of_subtreap->GetChild(LEFT_NODE_CHILD), table, next_code);
        }
        if (root_of_subtreap->GetChild(RIGHT_NODE_CHILD)) {
            VariableLenghCode   next_code(scrached_word.code | (1 << scrached_word.lengh), scrached_word.lengh + 1);
            FillTableOnSubtreap(root_of_subtreap->GetChild(RIGHT_NODE_CHILD), table, next_code);
        }
    }
}

const char *treap_exceptions::InvalidPathException::what() const noexcept {
    return "Invalid bit given, couldnt get this node";
}

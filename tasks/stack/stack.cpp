#include "stack.h"

Node::Node(int32_t value, Node* previous) : value(value), previous(previous) {
}

Stack::Stack() : head_(nullptr), size_(0) {
}

Stack::~Stack() {
    while (head_) {
        Node* next = head_->previous;
        delete head_;
        head_ = next;
    }
}

void Stack::Push(int32_t value) {
    head_ = new Node(value, head_);
    size_++;
}

void Stack::Pop() {
    Node* next_head = head_->previous;
    delete head_;
    head_ = next_head;
    size_--;
}

int32_t Stack::Top() const {
    return head_->value;
}

int32_t Stack::Size() const {
    return size_;
}

void Stack::Clear() {
    while (size_) {
        Pop();
    }
}

bool Stack::Empty() const {
    return !size_;
}

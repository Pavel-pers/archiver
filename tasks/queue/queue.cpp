#include "queue.h"

Node::Node(int32_t value, Node* next) : value(value), next(next) {
}

Queue::Queue() {
    front_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}

Queue::~Queue() {
    while (front_) {
        Node* next = front_->next;
        delete front_;
        front_ = next;
    }
}

void Queue::Push(int32_t value) {
    Node* newtail = new Node(value, nullptr);
    if (size_ != 0) {
        tail_->next = newtail;
    } else {
        front_ = newtail;
    }

    tail_ = newtail;
    size_++;
}

void Queue::Pop() {
    Node* next = front_->next;
    delete front_;
    front_ = next;
    size_--;
}

int32_t Queue::Front() const {
    return front_->value;
}

int32_t Queue::Size() const {
    return size_;
}

void Queue::Clear() {
    while (size_) {
        Pop();
    }
}

bool Queue::Empty() const {
    return !size_;
}

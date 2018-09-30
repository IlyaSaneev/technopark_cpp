#include "Queue.h"
#include <iostream>
#include <stdexcept>

Queue::Queue(size_t queue_size)
    : buffer_(nullptr), queue_size_(queue_size), head_(0), tail_(0) {
  buffer_ = (int*)malloc(queue_size_ * sizeof(int));
  // buffer_ = (int*)malloc(queue_size_);
  // TODO: case 3 - invalid read/write
  memset(buffer_, 0, sizeof(decltype(buffer_[0])) * queue_size);
}

Queue::~Queue() {
  free(buffer_);
}

void Queue::Push(int val) {
  if ((tail_ + 1) % queue_size_ == head_) {
    throw std::runtime_error("queue is full");
  }
  // invalid write
  buffer_[tail_++] = val;
  if (tail_ == queue_size_)
    tail_ = 0;
}

int Queue::Pop() {
  if (Empty()) {
    throw std::runtime_error("queue is empty");
  }
  // invalid read
  int out = buffer_[head_++];
  if (head_ == queue_size_)
    head_ = 0;
  return out;
}

int Queue::Back() const {
  if (Empty())
    throw std::runtime_error("queue is empty");

  if (tail_ == 0)
    return buffer_[queue_size_ - 1];

  return buffer_[tail_ - 1];
}

int Queue::Front() const {
  if (Empty())
    throw std::runtime_error("queue is empty");
  return buffer_[head_];
}

#include "Stack.h"
#include <iostream>
#include <stdexcept>

Stack::Stack(size_t stack_size) : buffer_(0), stack_size_(stack_size), top_(-1) {
  buffer_ = new int[stack_size_];
  memset(buffer_, 0, sizeof(decltype(buffer_[0])) * stack_size_);
}

Stack::~Stack() {
  delete[] buffer_;
}

void Stack::Push(int val) {
  if (top_ + 1 >= stack_size_) {
    throw std::runtime_error("stack overflow");
  }
  buffer_[++top_] = val;
}

size_t Stack::Size() const {
  return top_ + 1;
}

int Stack::Pop() {
  if (top_ == -1) {
    throw std::runtime_error("stack underflow");
  }

  return buffer_[top_--];
}

int Stack::Top() const {
  return buffer_[top_];
}

void Stack::Display() {
  for (int i = 0; i <= top_; ++i) {
    std::cout << buffer_[i] << "\t";
  }

  std::cout << std::endl;
}

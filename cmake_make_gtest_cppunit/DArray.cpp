#include "DArray.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

DArray::DArray(size_t buffer_size)
    : buffer_(nullptr), buf_size_(buffer_size), actual_size_(0) {
  buffer_ = (double *)malloc(buf_size_ * sizeof (double));
  memset(buffer_, 0, sizeof(decltype(buffer_[0])) * buf_size_);
}

void DArray::DtorHelper() {
  // free(buffer_); // TODO: case 2 - double free
}

DArray::~DArray() {
  free(buffer_);
  DtorHelper();
  buffer_ = nullptr;

  // TODO: case 5 - invalid read/write after deallocation
  // printf("%f", buffer_[ 0 ]);
  // buffer_[ 0 ] = 10;
}

double DArray::At(int index) {
  if (!buffer_ || index > actual_size_ - 1) {
    throw std::runtime_error("out of range");
  }

  return buffer_[index];
}

void DArray::Reallocate() {
  size_t new_buf_size = buf_size_ * 2;
  auto new_buffer = new double[new_buf_size];
  memcpy(new_buffer, buffer_, buf_size_ * sizeof(double));
  // TODO: case 1 - memleak
  delete[] buffer_;
  buf_size_ = new_buf_size;
  buffer_ = new_buffer;
}

void DArray::PushBack(double value) {
  if (actual_size_ == buf_size_) {
    Reallocate();
  }

  buffer_[actual_size_++] = value;
}

double DArray::PopBack() {
  if (actual_size_ == 0)
    throw std::runtime_error("DArray is empty");

  return buffer_[--actual_size_];
}

double& DArray::operator[](size_t index) {
  return buffer_[index];
}

const double& DArray::operator[](size_t index) const {
  return buffer_[index];
}

void DArray::Display() {
  for (size_t i = 0; i < actual_size_; ++i) {
    std::cout << buffer_[i] << "\t";
  }
  std::cout << std::endl;
}

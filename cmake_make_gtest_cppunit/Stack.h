#ifndef LECTURE2_STACK_H_
#define LECTURE2_STACK_H_

#include <cstddef>

// #include "DArray.h" instead |int* buffer_;|

class Stack {
 public:
  explicit Stack(size_t size);
  virtual ~Stack();

  void Push(int val);
  int Pop();
  int Top() const;
  size_t Size() const;

  bool Empty() const { return top_ == -1; }

  void Display();

 private:
  int* buffer_;
  size_t stack_size_;
  size_t top_;
};

#endif  // LECTURE2_STACK_H_

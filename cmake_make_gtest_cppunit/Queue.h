#ifndef LESSON2_QUEUE_
#define LESSON2_QUEUE_

// http://ru.cppreference.com/w/cpp/container/queue

#include <cstddef>

class Queue {
 public:
  Queue(size_t size);
  virtual ~Queue();

  void Push(int val);
  
  // Pop == Dequeue
  int Pop();

  int Front() const;

  int Back() const;

  bool Empty() const { return head_ == tail_; }

 private:
  int* buffer_;
  size_t queue_size_;
  int head_;
  int tail_;
};

#endif  // LESSON2_QUEUE_

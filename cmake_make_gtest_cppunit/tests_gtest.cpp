#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "DArray.h"
#include "Heap.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"

TEST(DArray, SizeCapacity) {
  DArray darray(64);
  darray.PushBack(5);
  EXPECT_EQ(darray.Capacity(), 64);
  EXPECT_EQ(darray.Size(), 1);
}

TEST(DArray, Pop) {
  DArray darray(64);
  try {
    darray.PushBack(5);
    darray.PushBack(5);
    darray.PushBack(5);
    darray.PushBack(5);

    darray.PopBack();
    darray.PopBack();
    darray.PopBack();
    darray.PopBack();
    darray.PopBack();
  } catch(...) {
    return;
  }

  GTEST_FAIL() << " DArray should throw an exception";
}

TEST(Queue, Pop) {
  Queue queue(5);
  try {
    queue.Push(5);
    queue.Pop();
    queue.Push(10);
    queue.Push(20);  // queue contains only 20

    EXPECT_EQ(queue.Front(), 10) << "Queue Front should be 10";
    EXPECT_EQ(queue.Back(), 20) << "Queue Back should be 20";

    queue.Push(40);  // queue contains 10 20 40
    queue.Push(60);  // queue contains 20 40 60
    queue.Pop();
    queue.Push(80);  // queue contains 40 60 80

    EXPECT_EQ(queue.Back(), 80) << "Queue Front should be 80";
  } catch (const std::exception& e) {
    GTEST_FAIL() << e.what();
  }
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
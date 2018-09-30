#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>

#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "DArray.h"
#include "Heap.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"

class TestContainers : public CppUnit::TestCase {
 public:
  void TestDArray() {
    DArray d(64);
    d.PushBack(12.5);
    d.PushBack(8.2);
    d.PushBack(1.6);
    CPPUNIT_ASSERT_MESSAGE("DArray size should be equal to 3", d.Size() == 3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("DArray capacity should be equal to 64", (size_t)64,
                                 d.Capacity());
  }

  void TestLinkedList() {
    LinkedList l1;
    l1.PushBack(5);
    l1.PushBack(10);
    l1.PushBack(15);

    Node* element = l1.Search(10);
    CPPUNIT_ASSERT_MESSAGE("The search item 10 was not found",
                           element != nullptr);
  }

  void TestStack() {
    Stack stack(256);
    stack.Push(5);
    stack.Push(10);
    stack.Push(20);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stack size should be ", (size_t)3,
                                 stack.Size());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stack top should be ", 20, stack.Top());
    stack.Pop();
    stack.Pop();
    stack.Pop();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Stack should be empty", (size_t)0,
                                 stack.Size());
  }

  void TestQueue() {
    Queue queue(5);
    try {
      queue.Push(5);
      queue.Pop();
      queue.Push(10);
      queue.Pop();
      queue.Push(20);  // queue contains only 20

      CPPUNIT_ASSERT_EQUAL_MESSAGE("Queue Front should be 20", queue.Front(),
                                   20);
      CPPUNIT_ASSERT_EQUAL_MESSAGE("Queue Front should be 20", queue.Back(),
                                   20);

      queue.Push(40);  // queue contains 20 40
      queue.Push(60);  // queue contains 20 40 60
      queue.Pop();
      queue.Push(80);  // queue contains 80 60 40

      CPPUNIT_ASSERT_EQUAL_MESSAGE("Queue Front should be 40", queue.Back(),
                                   80);
    } catch (const std::exception& e) {
      CPPUNIT_FAIL(e.what());
    }
    std::cout << std::endl;
  }

  CPPUNIT_TEST_SUITE(TestContainers);
  CPPUNIT_TEST(TestDArray);
  CPPUNIT_TEST(TestLinkedList);
  CPPUNIT_TEST(TestStack);
  CPPUNIT_TEST(TestQueue);
  CPPUNIT_TEST_SUITE_END();
};

class TestHeap : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestHeap);
  CPPUNIT_TEST(TestBuildHeap);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp(void) final;
  void tearDown(void) final;

 protected:
  void TestBuildHeap();

 private:
  DArray* darray_ = nullptr;
};

//-----------------------------------------------------------------------------

void TestHeap::TestBuildHeap() {
  heap::MakeHeap(*darray_);
  double largest =
      heap::ExtractMax(*darray_);  // pop out maximum element from heap
  CPPUNIT_ASSERT_EQUAL_MESSAGE("Heap max should be 9", 9.0, largest);
}

void TestHeap::setUp() {
  darray_ = new DArray(64);
  //  DArray d(64);
  darray_->PushBack(1);
  darray_->PushBack(4);
  darray_->PushBack(9);
  darray_->PushBack(3);
  darray_->PushBack(1);
  darray_->PushBack(5);
}

void TestHeap::tearDown() {
  delete darray_;
}

//-----------------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION(TestHeap);
CPPUNIT_TEST_SUITE_REGISTRATION(TestContainers);

int main() {
  CppUnit::Test* test = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
  CppUnit::TextTestRunner runner;
  runner.addTest(test);
  runner.run();
  return 0;
}

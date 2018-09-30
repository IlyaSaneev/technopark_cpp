
#include <stdio.h>
#include <stdlib.h> /* srand, rand */
#include <sys/resource.h>
#include <time.h> /* time */
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "DArray.h"
#include "Heap.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"

// style : https://google.github.io/styleguide/cppguide.html
using namespace std;
void UseDArray() {
  std::cout << "--------- DArray ---------" << std::endl << std::endl;
  DArray d(3);
  d.PushBack(12.5);
  d.PushBack(8.2);
  d.PushBack(1.6);
  d.PushBack(1.6);  // here must be *nullptr
  d.PushBack(1.6);
  d.PushBack(1.6);
  d.PushBack(1.6);

  std::cout << "d at index 2 is " << d.At(2) << std::endl;
  try {
    std::cout << "d at index 3 is " << d.At(3) << std::endl;
  } catch (const std::exception& e) {
    std::cout << "error : " << e.what() << std::endl;
  }
  std::cout << std::endl;
}

void UseLinkedList() {
  std::cout << "--------- Linked List ---------" << std::endl << std::endl;
  LinkedList l1;
  l1.PushBack(5);
  l1.PushBack(10);
  l1.PushBack(15);

  l1.Display();

  Node* element = l1.Search(10);
  if (element) {
    Node* inserted = l1.InsertAfter(element, 100);
    l1.Display();
    l1.DeteleAt(inserted);
    l1.Display();
  }
}

void UseStack() {
  std::cout << "--------- Stack ---------" << std::endl << std::endl;
  Stack stack(256);
  try {
    stack.Push(5);
    stack.Push(10);
    stack.Push(20);

    stack.Display();
  } catch (const std::exception& e) {
    std::cout << "error : " << e.what() << std::endl;
  }
  std::cout << std::endl;
}

void UseQueue() {
  std::cout << "--------- Queue ---------" << std::endl << std::endl;
  Queue queue(5);
  try {
    queue.Push(5);
    queue.Pop();
    queue.Push(10);
    queue.Pop();
    queue.Push(20);  // queue contains only 20

    std::cout << "Front() = " << queue.Front() << std::endl;
    std::cout << "Back() = " << queue.Back() << std::endl;

    queue.Push(40);  // queue contains 20 40
    queue.Push(60);  // queue contains 60 40 20

    std::cout << "Front() = " << queue.Front() << std::endl;
    std::cout << "Back() = " << queue.Back() << std::endl;

    queue.Pop();
    queue.Push(80);  // queue contains 80 60 40 20
    queue.Push(80);
    queue.Push(80);
    queue.Push(80);
    queue.Push(80);
    std::cout << "Front() = " << queue.Front() << std::endl;
    std::cout << "Back() = " << queue.Back() << std::endl;
  } catch (const std::exception& e) {
    std::cout << "error : " << e.what() << std::endl;
  }
  std::cout << std::endl;
}

void UseHeap() {
  std::cout << "--------- Heap ---------" << std::endl << std::endl;
  DArray d(64);
  d.PushBack(3);
  d.PushBack(1);
  d.PushBack(4);
  d.PushBack(1);
  d.PushBack(5);
  d.PushBack(9);
  std::cout << "d: ";
  d.Display();
  heap::MakeHeap(d);
  std::cout << "d: ";
  d.Display();
  double largest = heap::ExtractMax(d);  // pop out maximum element from heap
  std::cout << "largest element: " << largest << '\n';
  std::cout << "heap without largest: ";
  d.Display();
  heap::Add(d, 20);
  std::cout << "heap after add : ";
  d.Display();
  heap::Add(d, 6);
  std::cout << "heap after add : ";
  d.Display();
}

void CheckValgrindCondJump() {
  // TODO: case 4 - conditional jump or move depends on uninitialised value(s)
  char* data = (char*)malloc(5);
  data[0] = 'a';
  data[1] = 'b';
  data[1] = 'c';
  if (data[2] == 'c') {
    printf("YES\n");
  }
  free(data);
}

void CheckValgrindAutoVariables() {
  // TODO: case 6 - local variables
  char dest[2];
  char src[2];
  src[0] = 'a';
  src[1] = 'b';
  for (int i = 0; i <= 2; i++)
    dest[i] = src[i];
  printf("%s\n", dest);
}

int main() {
  //
  UseDArray();
  //////////////////////////////////////////////////////////////////////////
  UseLinkedList();
  //////////////////////////////////////////////////////////////////////////
  UseStack();
  //////////////////////////////////////////////////////////////////////////
  UseQueue();
  //////////////////////////////////////////////////////////////////////////
  UseHeap();

  CheckValgrindCondJump();

  CheckValgrindAutoVariables();

  return 0;
}

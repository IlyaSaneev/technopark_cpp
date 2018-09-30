#ifndef LECTURE2_LINKEDLIST_H_
#define LECTURE2_LINKEDLIST_H_

struct Node {
  Node()
      : data(0),
        next(nullptr),
        prev(nullptr) {}

  int data;
  Node* next;
  Node* prev;
};

// http://ru.cppreference.com/w/cpp/container/list

class LinkedList {
 public:
  LinkedList();
  virtual ~LinkedList();

  void PushBack(int val);

  void Display();

  Node* Front() const { return head_; };
  Node* Back() const { return tail_; };

  Node* Search(int val);

  Node* InsertAfter(Node* node, int data);

  bool DeteleAt(Node* node);

  static void Union(LinkedList* list1, LinkedList* list2);

 private:
  Node* head_;
  Node* tail_;
};

#endif  // LECTURE2_LINKEDLIST_H_

#include "LinkedList.h"
#include <iostream>

LinkedList::LinkedList() : head_(nullptr), tail_(nullptr) {}

LinkedList::~LinkedList() {
  Node* current = head_;
  while (current != nullptr) {
    Node* next = current->next;
    delete [] current;
    current = next;
  }
}

void LinkedList::PushBack(int val) {
  Node* temp = new Node();
  temp->data = val;
  temp->next = nullptr;
  temp->prev = nullptr;
  if (head_ == nullptr) {
    head_ = temp;
    tail_ = temp;
  } else {
    tail_->next = temp;
    temp->prev = tail_;
    tail_ = temp;
  }
}

void LinkedList::Display() {
  Node* current = head_;
  while (current != nullptr) {
    std::cout << current->data << "\t";
    current = current->next;
  }
  std::cout << std::endl;
}

Node* LinkedList::Search(int val) {
  Node* current = head_;
  while (current != nullptr) {
    if (current->data == val) {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

Node* LinkedList::InsertAfter(Node* node, int data) {
  if (node == nullptr)
    return nullptr;
  auto new_node = new Node();
  new_node->data = data;
  new_node->prev = node;  //
  new_node->next = node->next;

  if (new_node->next != nullptr) {
    new_node->next->prev = new_node;
  }

  node->next = new_node;
  return new_node;
}

bool LinkedList::DeteleAt(Node* node) {
  if (node == nullptr)
    return false;

  if (node->next != nullptr) {
    node->next->prev = node->prev;
  }

  if (node->prev != nullptr) {
    node->prev->next = node->next;
  }

  return true;
}

void LinkedList::Union(LinkedList* list1, LinkedList* list2) {
  if (list1 == nullptr || list2 == nullptr)
    return;

  Node* tail1 = list1->Back();
  Node* head2 = list2->Front();

  tail1->next = head2;
  head2->prev = tail1;
}

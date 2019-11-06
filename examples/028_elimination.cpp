#include <iostream>

struct A {};

struct B : public A {
  int a;
};

struct C : public A {
  A a;
  int var;
};

struct D : public A {
  int a;
  B b;
};

int main() {
  std::cout << sizeof(A) << std::endl;
  std::cout << sizeof(B) << std::endl;
  std::cout << sizeof(C) << std::endl;
  std::cout << sizeof(D) << std::endl;
  return 0;
}

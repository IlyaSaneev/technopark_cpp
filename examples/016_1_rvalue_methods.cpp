#include <iostream>

struct S {
  void f() &{ std::cout << "lvalue\n"; }
  void f() &&{ std::cout << "rvalue\n"; }
};

int main() {
  S s;
  s.f();            // prints "lvalue"
  std::move(s).f(); // prints "rvalue"
  S().f();          // prints "rvalue"
}
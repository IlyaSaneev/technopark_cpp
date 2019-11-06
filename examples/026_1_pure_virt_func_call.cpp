#include <iostream>

using namespace std;

struct A {

};

struct B : public A {

};

struct AA {
  AA() {
  //  bar();
  }
  virtual void bar() = 0;
  virtual A* test() {
    cout << "test - A" << endl;
    return new A{};
  };
};

struct BB : public AA {
  BB() {
    bar();
  }
  virtual void foo(){std::cout << "BB" << std::endl;};
  void bar() override {std::cout << "bar" << std::endl;};
  B* test() override {
    cout << "test - B" << endl;
    return new B{};
  };
};

struct CC : public BB {
  void foo() {std::cout << "CC" << std::endl;};
};

int main() {
//  AA* aa = new BB{};
//  aa->test();
  return 0;
}

#include <iostream>

namespace diamond_problem {

struct N0 {
  N0() {
    std::cout << "ctor N0" << std::endl;
  }
};


class A /*: public N0*/ {
 public:
  A() { std::cout << "ctor A()" << std::endl; }
  explicit A(int a) { std::cout << "ctor A() " << " a = " << a << std::endl; }

  virtual void foo() { std::cout << "foo1" << std::endl << std::endl; }

  int a = 0;
  int b = 0;
};

struct Bfirst {
  Bfirst() {
    std::cout << "ctor Bfirst()" << std::endl;
  }
};

class B :  public Bfirst, public virtual A {
 public:
  B() { std::cout << "ctor B()" << std::endl; }
  explicit B(int a) : A(a) { std::cout << "ctor B() " << " a = " << a << std::endl; }
};

class C :  public virtual A {
 public:
  C() { std::cout << "ctor C()" << std::endl; }
};

/*
class AA {
 public:
  AA() { std::cout << "ctor AA()" << std::endl; }
  explicit AA(int a) { std::cout << "ctor AA() " << " a = " << a << std::endl; }
  int a = 0;
  int b = 0;
};

class BB :  public virtual AA {
 public:
  BB() { std::cout << "ctor BB()" << std::endl; }
  explicit BB(int a) : AA(a) { std::cout << "ctor BB() " << " a = " << a << std::endl; }
};

class CC :  public virtual AA {
 public:
  CC() { std::cout << "ctor CC()" << std::endl; }
};
*/

class NonVirtual {
 public:
  NonVirtual() { std::cout << "ctor Nonvirtual()" << std::endl; }
};

class D : public NonVirtual, public B, public C/*, public BB, public CC*/ {
 public:
  D() : /*A(5),*/ B(5) { std::cout << "ctor D()" << std::endl; }
};
} // namespace diamond_problem

namespace another_inh_problem {
class B {
 public:
  B() { std::cout << "ctor B()" << std::endl; }
  virtual void foo() { std::cout << "foo2" << std::endl << std::endl; }
};

class C {
 public:
  C() { std::cout << "ctor C()" << std::endl; }
  virtual void foo() { std::cout << "foo3" << std::endl << std::endl; }
};

class D : public B, public C {
 public:
  using C::foo;
  D() { std::cout << "ctor D()" << std::endl; }
};
} // namespace another_inh_problem

int main() {
  {
    using namespace diamond_problem;
    D d1;
    d1.foo();

//    std::cout << sizeof(diamond_problem::A) << std::endl;
//    std::cout << sizeof(diamond_problem::B) << std::endl;
//    std::cout << sizeof(diamond_problem::C) << std::endl;
//    std::cout << sizeof(diamond_problem::D) << std::endl;
//    std::cout << std::endl;
  }
//  std::cout << std::endl;
//  {
//    another_inh_problem::D d;
//    d.foo();
//  }
  return 0;
}

// Run with and without -fno-elide-constructors
#include <iostream>

using namespace std;

struct A {
    A() {
        std::cout << "A()" << std::endl;
    }

    A(A &&a) {
        std::cout << "A(A &&)" << std::endl;
    }

    A(A const &a) {
        std::cout << "A(A const &)" << std::endl;
    }

    A &operator=(A const &a) {
        std::cout << "A &operator=(A const &a)" << std::endl;
        return *this;
    }
};

A example_RVO(A a) {
    std::cout << "inside example_RVO" << std::endl;
    return A(a);
}

A example_NRVO(A a) {
    std::cout << "inside example_NRVO" << std::endl;
    A newA(a);
    std::cout << &newA << std::endl;
    return newA;
}

int main(int argc, char *argv[]) {
    A a1;
    std::cout << "before example_RVO" << std::endl;
    A a2 = example_RVO(a1);
    std::cout << "between example_RVO and example_NRVO" << std::endl;
    A a3 = example_NRVO(a2);
    std::cout << &a3 << std::endl;
    std::cout << "after example_NRVO" << std::endl;
    return 0;
}

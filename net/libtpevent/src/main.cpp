#include <iostream>
#include "server.hpp"

int main(int argc, char* argv[]) {
  try {
    Options opt(argc, argv);
    Server serv(opt);
    serv.run();
    return 0;
  } catch (std::exception& e) {
    std::cerr << "Terminated: " << e.what() << std::endl;
  }
  return 0;
}

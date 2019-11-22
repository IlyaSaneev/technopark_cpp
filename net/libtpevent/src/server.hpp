#include <memory>

#include "engine.hpp"
#include "options.hpp"

class Server {
 public:
  Server(const Options& opt);
  void run();

 private:
  std::unique_ptr<Engine> m_Engine;
};

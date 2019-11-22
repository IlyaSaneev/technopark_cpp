
#include "engine.hpp"

class EpollEngine : public Engine {
 public:
  explicit EpollEngine(int port) : Engine(port) {}
  virtual void run() override;

 private:
  void eventLoop();
};

#include <vector>

#include "engine.hpp"

class PollEngine : public Engine {
 public:
  explicit PollEngine(int port) : Engine(port) {}
  virtual void run() override;

 private:
  void acceptNewConnections();
  void manageConnections();

 private:
  std::vector<Client> m_Clients;
};

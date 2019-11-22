#include <vector>

#include "engine.hpp"

class SelectEngine : public Engine {
 public:
  explicit SelectEngine(int port) : Engine(port) {}
  virtual void run() override;

 private:
  void manageConnections();

 private:
  std::vector<Client> m_Clients;
};

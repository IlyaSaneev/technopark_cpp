#ifndef _ENGINE_HPP
#define _ENGINE_HPP

#include "stdint.h"

enum class client_state_t : uint8_t { WANT_READ, WANT_WRITE };

/*
    Client - is just an sd and state
*/
struct Client {
  explicit Client(int _sd) : sd(_sd), state(client_state_t::WANT_READ) {}
  Client(int _sd, client_state_t _state) : sd(_sd), state(_state) {}

  int sd;
  client_state_t state;
};

class Engine {
 public:
  explicit Engine(int port);
  virtual ~Engine();

  virtual void run() = 0;

  int listener() const { return m_Listener; }

 private:
  int listenSocket(uint32_t port, uint32_t listen_queue_size);

 private:
  int m_Listener;
};

#endif  // _ENGINE_HPP

#include <errno.h>
#include <netinet/in.h>  // htons(), INADDR_ANY
#include <stdint.h>      // uint32_t
#include <string.h>      // memset()
#include <sys/socket.h>  // socket()
#include <unistd.h>      // close()

#include <stdexcept>
#include <string>

#include "engine.hpp"

static const uint32_t kListenQueueSize = 25;

Engine::Engine(int port) {
  m_Listener = listenSocket(port, kListenQueueSize);
  if (m_Listener <= 0)
    throw std::runtime_error(std::string("error listen socket: ") +
                             strerror(errno));
}

Engine::~Engine() {
  close(m_Listener);
}

int Engine::listenSocket(uint32_t port, uint32_t listen_queue_size) {
  int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sd <= 0)
    return -1;

  int yes = 1;
  if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    return -1;

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  if (bind(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    return -1;
  }

  listen(sd, listen_queue_size);
  return sd;
}

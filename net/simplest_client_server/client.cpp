#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#define MESSAGE "This is my first message!"

void write_to_server(int filedes) {
  int nbytes;
  nbytes = write(filedes, MESSAGE, strlen(MESSAGE) + 1);
  if (nbytes < 0) {
    throw std::runtime_error("write failed: " + std::string(strerror(errno)));
  }
  printf("write message %s to socket \n", MESSAGE);
}

void read_from_server(int filedes) {
  char buf[1024];
#ifdef __APPLE__
  // mac os x don't defines MSG_NOSIGNAL
  int n = ::recv(filedes, buf, sizeof(buf), 0);
#else
  int n = ::recv(m_Sd, buf, sizeof(buf), MSG_NOSIGNAL);
#endif

  if (-1 == n && errno != EAGAIN)
    throw std::runtime_error("read failed: " + std::string(strerror(errno)));
  if (0 == n)
    throw std::runtime_error("client: " + std::to_string(filedes) +
                             " disconnected");
  if (-1 == n)
    throw std::runtime_error("client: " + std::to_string(filedes) +
                             " timeouted");

  std::string ret(buf, buf + n);
  while (ret.back() == '\r' || ret.back() == '\n')
    ret.pop_back();
  std::cerr << "client: " << filedes << ", recv: \n"
            << ret << " [" << n << " bytes]" << std::endl;
}

void init_sockaddr(struct sockaddr_in* name,
                   const char* hostname,
                   uint16_t port) {
  struct hostent* hostinfo;

  name->sin_family = AF_INET;

  // htons -- host to net short
  // htonl -- host to net long
  name->sin_port = htons(port);
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr*)hostinfo->h_addr;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " host port" << std::endl;
    return 0;
  }
  int sock = 0;
  struct sockaddr_in servername;
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket (client)");
    exit(EXIT_FAILURE);
  }
  std::string host(argv[1]);
  int port = std::stoi(argv[2]);
  init_sockaddr(&servername, host.c_str(), port);
  if (0 > connect(sock, (struct sockaddr*)&servername, sizeof(servername))) {
    perror("connect (client)");
    exit(EXIT_FAILURE);
  }

  write_to_server(sock);
  read_from_server(sock);

  close(sock);
  exit(EXIT_SUCCESS);
}

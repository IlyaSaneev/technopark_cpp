/*
    Цикл мультиплексирования на poll().
    Особенности:
        - Надо дочитывать до конца! Если в буфере остануться данные, то при
   следующем ожидании POLLIN poll() вернет этот сокет немедленно.

*/

#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "poll.hpp"

void PollEngine::acceptNewConnections() {
  int timeout = 3000;  // msec

  struct pollfd fd;
  fd.fd = listener();
  fd.events = POLLIN;
  fd.revents = 0;

  while (1) {
    int poll_ret = poll(&fd, 1, /* timeout in msec */ timeout);

    if (poll_ret == 0) {
      continue;
    } else if (poll_ret == -1) {
      throw std::runtime_error(std::string("poll: ") + strerror(errno));
    }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    socklen_t cli_len = sizeof(client);

    int cli_sd = accept(listener(), (struct sockaddr*)&client, &cli_len);
    std::cerr << "new client: " << cli_sd << "\n";

    // TODO: need lock
    m_Clients.push_back(Client(cli_sd));
  }
}

void PollEngine::manageConnections() {
  struct pollfd fds[32768];  // 2^15

  while (1) {
    std::vector<int> disconnected_clients;

    if (m_Clients.empty()) {
      // no clients
      usleep(1000);
      continue;
    }

    for (size_t i = 0; i < m_Clients.size(); ++i) {
      fds[i].fd = m_Clients[i].sd;

      if (m_Clients[i].state == client_state_t::WANT_READ)
        fds[i].events = POLLIN;
      else
        fds[i].events = POLLOUT;
      fds[i].revents = 0;
    }

    int poll_ret = poll(fds, m_Clients.size(), /* timeout in msec */ 100);

    if (poll_ret == 0) {
      // nothing activity from any clients
      continue;
    } else if (poll_ret == -1) {
      throw std::runtime_error(std::string("poll: ") + strerror(errno));
    }

    for (size_t i = 0; i < m_Clients.size(); ++i) {
      if (fds[i].revents == 0)
        continue;

      if (fds[i].revents & POLLHUP) {
        // e.g. previous write() was in a already closed sd
        std::cerr << "client hup\n";
        disconnected_clients.push_back(fds[i].fd);
      } else if (fds[i].revents & POLLIN) {
        if (m_Clients[i].state != client_state_t::WANT_READ)
          continue;

        char buf[256];
        int r = read(fds[i].fd, buf, sizeof(buf));

        if (r < 0) {
          std::cerr << "some read error!\n";
          close(fds[i].fd);
          disconnected_clients.push_back(fds[i].fd);
          continue;
        }
        buf[r] = '\0';

        std::string tmp(buf);
        while (tmp[tmp.size() - 1] == '\r' || tmp[tmp.size() - 1] == '\n')
          tmp.pop_back();

        if (r > 0) {
          std::cerr << "read: " << r << " bytes [" << tmp.c_str() << "]\n";
          m_Clients[i].state = client_state_t::WANT_WRITE;
        }

        if (r == 0) {
          std::cerr << "client disconnected: " << fds[i].fd << "\n";
          close(fds[i].fd);
          disconnected_clients.push_back(fds[i].fd);
        }
      } else if (fds[i].revents & POLLOUT) {
        if (m_Clients[i].state != client_state_t::WANT_WRITE)
          continue;

        char buf[] = "hello from server!\n";
        write(fds[i].fd, buf, sizeof(buf));
        m_Clients[i].state = client_state_t::WANT_READ;
      } else if (fds[i].revents & POLLNVAL) {
        // e.g. if set clos'ed descriptor in poll
        std::cerr << "POLLNVAL !!! need remove this descriptor: " << fds[i].fd
                  << "\n";
        disconnected_clients.push_back(fds[i].fd);
      } else {
        if (fds[i].revents & POLLERR)
          std::cerr << "WARNING> revents = POLLERR. [SD = " << fds[i].fd
                    << "]\n";
        else
          std::cerr << "WARNIG> revent = UNKNOWN_EVENT: " << fds[i].revents
                    << " [SD = " << fds[i].fd << "]\n";
      }
    }

    // remove disconnected clients
    for (size_t i = 0; i < disconnected_clients.size(); ++i) {
      int sd = disconnected_clients[i];
      auto f = [&sd](const Client& state) { return state.sd == sd; };
      auto iterator = std::find_if(m_Clients.begin(), m_Clients.end(), f);
      assert(iterator != m_Clients.end());
      m_Clients.erase(iterator);
    }
  }
}

void PollEngine::run() {
  std::cerr << "poll server starts" << std::endl;

  std::thread t1(std::bind(&PollEngine::acceptNewConnections, this));
  std::thread t2(std::bind(&PollEngine::manageConnections, this));

  t1.join();
  t2.join();
}

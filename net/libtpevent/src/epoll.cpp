#include <netinet/in.h>   // htons(), INADDR_ANY
#include <netinet/tcp.h>  // SOL_TCP
#include <sys/epoll.h>
#include <sys/socket.h>  // socket()

#include <assert.h>
#include <errno.h>

#include <unistd.h>  // close()

#include <stdint.h>  // uint32_t
#include <string.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include "epoll.hpp"

static bool set_desire_events(int epfd, int cli_fd, int events, Client* c) {
  struct epoll_event ev;
  ev.data.ptr = c;
  ev.events = events;
  bool ret = (epoll_ctl(epfd, EPOLL_CTL_MOD, cli_fd, &ev) == 0);
  return ret;
}

static void accept_action(int epfd, int listener) {
  struct sockaddr_in client;
  memset(&client, 0, sizeof(client));
  socklen_t cli_len = sizeof(client);

  int cli_sd = accept(listener, (struct sockaddr*)&client, &cli_len);

  // int yes = 1;
  // int keepidle = 10;  // sec
  // setsockopt(cli_sd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
  // setsockopt(cli_sd, SOL_TCP, TCP_KEEPCNT, &yes, sizeof(yes));
  // setsockopt(cli_sd, SOL_TCP, TCP_KEEPIDLE, /*sec*/&keepidle,
  // sizeof(keepidle)); setsockopt(cli_sd, SOL_TCP, TCP_KEEPINTVL, &yes,
  // sizeof(yes));

  struct epoll_event cli_ev;
  // cli_ev.data.fd = cli_sd;
  cli_ev.data.ptr = new Client(cli_sd);

  // TODO: EPOLLONESHOT / EPOLLEXCLUSIVE / Thundering herd problem
  cli_ev.events = EPOLLIN;  // level-triggered, by default
  // cli_ev.events = EPOLLIN | EPOLLOUT | EPOLLET;     // ET - edge-triggered

  std::cerr << "new client: " << cli_sd << ", ptr: " << (void*)cli_ev.data.ptr
            << "\n";
  epoll_ctl(epfd, EPOLL_CTL_ADD, cli_sd, &cli_ev);
}

void EpollEngine::eventLoop() {
  const int max_epoll_clients = 32768;  // 2^15

  // создать дескриптор epoll
  int epfd = epoll_create(/*max cli*/ max_epoll_clients);

  {
    // Заполнить epoll_event структуру нужными событиями и контекстами
    // соединений для сервера

    //    typedef union epoll_data {
    //        void    *ptr;           /*Context */
    //        int      fd;
    //        uint32_t u32;
    //        uint64_t u64;
    //    } epoll_data_t;
    //
    //    struct epoll_event {
    //        uint32_t     events;    /* Epoll events */
    //        epoll_data_t data;      /* User data variable */
    //    };

    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = listener();
    ev.events = EPOLLIN;

    // Добавление дискриптора в список наблюдаемых
    epoll_ctl(epfd, EPOLL_CTL_ADD, listener(), &ev);
  }

  // Создать epoll_event для клиентов
  struct epoll_event* events = (struct epoll_event*)malloc(
      max_epoll_clients * sizeof(struct epoll_event));

  while (true) {
    std::vector<Client*> disconnected_clients;
    int maxevents = max_epoll_clients;  // количество событий за раз
    int epoll_ret = epoll_wait(epfd, events, maxevents, -1);

    if (epoll_ret == 0)
      continue;
    if (epoll_ret == -1)
      throw std::runtime_error(std::string("epoll: ") + strerror(errno));

    for (int i = 0; i < epoll_ret; ++i) {
      if (events[i].data.fd == listener()) {
        accept_action(epfd, listener());
        continue;
      }

      if (events[i].events & EPOLLHUP) {
        // e.g. previous write() was in a already closed sd
        std::cerr << "client hup\n";
        Client* cs = static_cast<Client*>(events[i].data.ptr);
        disconnected_clients.push_back(cs);
      } else if (events[i].events & EPOLLIN) {
        Client* cs = static_cast<Client*>(events[i].data.ptr);
        if (cs->state != client_state_t::WANT_READ)
          continue;

        int sock = cs->sd;

        char buf[256];
        ssize_t r = read(sock, buf, sizeof(buf));

        if (r < 0) {
          std::cerr << "some read error!\n";
          disconnected_clients.push_back(cs);
          continue;
        }
        buf[r] = '\0';

        std::string tmp(buf);
        while (tmp[tmp.size() - 1] == '\r' || tmp[tmp.size() - 1] == '\n')
          tmp.pop_back();

        if (r > 0) {
          std::cerr << "read: " << r << " bytes [" << tmp.c_str() << "]\n";
          cs->state = client_state_t::WANT_WRITE;

          bool ret = set_desire_events(epfd, sock, EPOLLOUT, cs);
          if (!ret)
            throw std::runtime_error(std::string("epoll_ctl error: ") +
                                     strerror(errno));
        } else if (r == 0) {
          std::cerr << "client disconnected: " << sock << "\n";
          disconnected_clients.push_back(cs);
        }
      } else if (events[i].events & EPOLLOUT) {
        Client* cs = static_cast<Client*>(events[i].data.ptr);
        if (cs->state != client_state_t::WANT_WRITE)
          continue;

        char buf[] = "hello from server!\n";
        write(cs->sd, buf, sizeof(buf));
        cs->state = client_state_t::WANT_READ;

        bool ret = set_desire_events(epfd, cs->sd, EPOLLIN, cs);
        if (!ret)
          throw std::runtime_error(std::string("epoll_ctl error: ") +
                                   strerror(errno));
      } else {
        Client* cs = (Client*)events[i].data.ptr;
        if (events[i].events & EPOLLERR)
          std::cerr << "WARNING> events = EPOLLERR. [SD = " << cs->sd << "]\n";
        else
          std::cerr << "WARNING> event = UNKNOWN_EVENT: " << events[i].events
                    << " [SD = " << cs->sd << "]\n";
      }
    }  // for (int i = 0; i < epoll_ret; ++i)

    // remove disconnected clients
    if (disconnected_clients.size()) {
      std::cerr << "GC start. disconnect: " << disconnected_clients.size()
                << " clients" << std::endl;
      for (size_t i = 0; i < disconnected_clients.size(); ++i) {
        int sd = disconnected_clients[i]->sd;
        std::cerr << "close: " << sd
                  << ", delete: " << (void*)disconnected_clients[i]
                  << std::endl;
        close(sd);
        delete disconnected_clients[i];
      }
    }
  }
}

void EpollEngine::run() {
  std::cerr << "epoll server starts" << std::endl;
  eventLoop();
}

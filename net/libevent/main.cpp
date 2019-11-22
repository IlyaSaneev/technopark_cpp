#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../common/socket.hpp"

static void echo_read_cb(struct bufferevent* buf_ev, void* arg) {
  struct evbuffer* buf_input = bufferevent_get_input(buf_ev);
  struct evbuffer* buf_output = bufferevent_get_output(buf_ev);

  {
    // Данные просто копируются из буфера ввода в буфер вывода
    // evbuffer_add_buffer(buf_output, buf_input);
  }

  size_t len;
  char* request_line = evbuffer_readln(buf_input, &len, EVBUFFER_EOL_CRLF);
  if (!request_line) {
    // The first line has not arrived yet
  } else {
    pthread_t tid = pthread_self();
    std::cout << "thread id : " << tid;

    std::cerr << "read: " << request_line << std::endl;
    std::string ans = std::string("echo: ") + request_line + "\r\n";
    evbuffer_add(buf_output, ans.data(), ans.size());
    free(request_line);
  }
}

static void echo_event_cb(struct bufferevent* buf_ev, short events, void* arg) {
  if (events & BEV_EVENT_ERROR)
    std::cerr << "bufferevent error: " << strerror(errno) << std::endl;

  if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
    std::cerr << "client close connection" << std::endl;
    bufferevent_free(buf_ev);
  }
}

static void accept_connection_cb(struct evconnlistener* listener,
                                 evutil_socket_t fd,
                                 struct sockaddr* addr,
                                 int sock_len,
                                 void* arg) {
  /* При обработке запроса нового соединения необходимо создать для него
     объект bufferevent */

  std::cerr << "accepted from: "
            << int2ipv4(((struct sockaddr_in*)addr)->sin_addr.s_addr)
            << std::endl;

  struct event_base* base = evconnlistener_get_base(listener);
  struct bufferevent* buf_ev =
      bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

  bufferevent_setcb(buf_ev, echo_read_cb, NULL, echo_event_cb, NULL);
  bufferevent_enable(buf_ev, (EV_READ | EV_WRITE));
}

static void accept_error_cb(struct evconnlistener* listener, void* arg) {
  struct event_base* base = evconnlistener_get_base(listener);
  int error = EVUTIL_SOCKET_ERROR();

  std::cerr << "listener error: " << error << " ("
            << evutil_socket_error_to_string(error) << ")" << std::endl;
  event_base_loopexit(base, NULL);
}

int main(int argc, char** argv) {
  int port = 5001;
  if (argc > 1)
    port = atoi(argv[1]);
  if (port < 0 || port > 65535) {
    std::cerr << "bad port" << std::endl;
    return -1;
  }

  struct event_base* base = event_base_new();
  if (!base) {
    std::cerr << "error to create event_loop base" << std::endl;
    return -1;
  }

  const char** methods = event_get_supported_methods();
  printf("Starting Libevent %s.  Available methods are:\n",
         event_get_version());
  for (int i = 0; methods[i] != NULL; ++i) {
    printf("    %s\n", methods[i]);
  }

  printf("default method for Libevent is %s\n", event_base_get_method(base));

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;  // работа с доменом IP-адресов
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sin.sin_port = htons(port);

  // http://www.wangafu.net/~nickm/libevent-book/Ref8_listener.html
  struct evconnlistener* listener =
      evconnlistener_new_bind(base, accept_connection_cb, NULL,
                              (LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE), -1,
                              (struct sockaddr*)&sin, sizeof(sin));

  if (!listener) {
    std::cerr << "error to create event listener" << std::endl;
    return -1;
  }

  std::cerr << "start listen on port: " << port << std::endl;

  evconnlistener_set_error_cb(listener, accept_error_cb);
  event_base_dispatch(base);
  return 0;
}

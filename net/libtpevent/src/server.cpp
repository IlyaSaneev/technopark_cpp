#include "server.hpp"
#include "poll.hpp"
#include "poll_async.hpp"
// #include "epoll.hpp"
#include "select.hpp"

namespace {
std::unique_ptr<Engine> get_engine(engine_t type, int port, bool async) {
  std::unique_ptr<Engine> ret;
  switch (type) {
    case engine_t::SELECT:
      ret.reset(new SelectEngine(port));
      break;
    case engine_t::POLL: {
      if (async)
        ret.reset(new AsyncPollEngine(port));
      else
        ret.reset(new PollEngine(port));
      break;
    }
#ifndef __APPLE__
    case engine_t::EPOLL: {
      if (async)
        ret.reset(new EpollEngine(port));
      else
        ret.reset(new EpollEngine(port));
      break;
    }
#endif  // __APPLE__
    case engine_t::UNKNOWN:
      throw std::logic_error("unknown engine type");
  }

  return ret;
}
}  // namespace

Server::Server(const Options& opt) {
  m_Engine = get_engine(opt.engine(), opt.port(), opt.async());
}

void Server::run() {
  m_Engine->run();
}

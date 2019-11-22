#include <iostream>
#include <sstream>
#include <stdexcept>

#include "options.hpp"
#include "utils.hpp"

namespace {
const std::string opt_port = "port";
const std::string opt_engine = "engine";
const std::string opt_async = "async";

void usage(const std::string& progname) {
  std::stringstream ss;
  ss << std::endl
     << "\tUsage: " << progname << " "
     << "--" << opt_port << "=port "
     << "--" << opt_engine << "=[select|poll] "
     << "--" << opt_async << "=[0|1]" << std::endl
     << std::endl;

  ss << "\t" << opt_port << ": 1025..65536" << std::endl
     << "\t" << opt_engine << ": engine for event loop" << std::endl
     << "\t" << opt_async << ": sync or async (threaded) engine" << std::endl
     << std::endl;

  std::cerr << ss.str() << std::endl;
}

engine_t string2engine(const std::string& s) {
  if (s == "select")
    return engine_t::SELECT;
  if (s == "poll")
    return engine_t::POLL;
  // if (s == "epoll") return engine_t::EPOLL;
  return engine_t::UNKNOWN;
}
}  // namespace

Options::Options(int count, const char* const* args) {
  if (count < 4) {
    usage(args[0]);
    throw std::runtime_error("bad options");
  }

  for (int i = 1; i < count; ++i) {
    std::vector<std::string> opt = utils::split(args[i], "=");
    if (2 != opt.size()) {
      usage(args[0]);
      throw std::runtime_error("bad options");
    }

    std::string opt_name = opt[0];
    if (utils::starts_with(opt_name, "--"))
      opt_name = opt_name.substr(2, std::string::npos);

    if (opt_name == opt_port) {
      m_Port = std::stoi(opt[1]);
    } else if (opt_name == opt_engine) {
      m_Engine = string2engine(opt[1]);
      if (m_Engine == engine_t::UNKNOWN) {
        usage(args[0]);
        throw std::runtime_error("bad options");
      }
    } else if (opt_name == opt_async) {
      int async = std::stoi(opt[1]);
      if (async == 0)
        m_Async = false;
      else if (async == 1)
        m_Async = true;
      else {
        usage(args[0]);
        throw std::runtime_error("bad options");
      }
    } else {
      usage(args[0]);
      throw std::runtime_error("bad options");
    }
  }
}

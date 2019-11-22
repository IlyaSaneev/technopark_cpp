#define BOOST_ASIO_HAS_STD_STRING_VIEW

#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <thread>

#include <boost/bind.hpp>
//#include <boost/function.hpp>

enum { max_len = 1024 };

void sync() {
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::socket s(io_service);
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query("localhost", "7789");

  boost::system::error_code ec;
  auto endpoint = resolver.resolve(query, ec);
  if (ec)
    throw std::runtime_error(ec.message());

  boost::asio::connect(s, endpoint, ec);
  if (ec)
    throw std::runtime_error(ec.message());

  std::string hello = "LOGIN\r\n";
  boost::asio::write(s, boost::asio::buffer(hello, hello.size()), ec);

  char reply[max_len];
  // size_t reply_length = boost::asio::read_some(s, boost::asio::buffer(reply,
  // max_len));
  s.read_some(boost::asio::buffer(reply, max_len), ec);
  std::cout << "Reply is: ";
  std::cout << reply << "\n";
}

void sync_http() {
  boost::asio::ip::tcp::iostream stream;
  stream.expires_from_now(boost::asio::chrono::seconds(60));
  stream.connect("www.boost.org", "http");
  stream << "GET /LICENSE_1_0.txt HTTP/1.0\r\n";
  stream << "Host: www.boost.org\r\n";
  stream << "Accept: */*\r\n";
  stream << "Connection: close\r\n\r\n";
  stream.flush();
  std::cout << stream.rdbuf();
}

void sync_http_full(const std::string& url) {
  try {
    boost::asio::ip::tcp::iostream s;

    // The entire sequence of I/O operations must complete within 60 seconds.
    // If an expiry occurs, the socket is automatically closed and the stream
    // becomes bad.
    s.expires_after(boost::asio::chrono::seconds(60));

    std::string::size_type pos = url.find("//");
    if (pos == std::string::npos)
      throw std::runtime_error("bad query");

    std::string host = url.substr(pos + 2, url.find("/", pos + 2) - pos - 2);
    if (host.empty())
      throw std::runtime_error("bad query");

    // Establish a connection to the server.
    s.connect(host.c_str(), "http");
    if (!s) {
      std::cout << "Unable to connect: " << s.error().message() << "\n";
      return;
    }

    // Send the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    s << "GET " << url.c_str() << " HTTP/1.0\r\n";
    s << "Host: " << host.c_str() << "\r\n";
    s << "Accept: */*\r\n";
    s << "Connection: close\r\n\r\n";

    // By default, the stream is tied with itself. This means that the stream
    // automatically flush the buffered output before attempting a read. It is
    // not necessary not explicitly flush the stream at this point.

    // Check that response is OK.
    std::string http_version;
    s >> http_version;
    unsigned int status_code;
    s >> status_code;
    std::string status_message;
    std::getline(s, status_message);
    if (!s || http_version.substr(0, 5) != "HTTP/") {
      std::cout << "Invalid response\n";
      return;
    }
    if (status_code != 200) {
      std::cout << "Response returned with status code " << status_code << "\n";
      return;
    }

    // Process the response headers, which are terminated by a blank line.
    std::string header;
    while (std::getline(s, header) && header != "\r")
      std::cout << header << "\n";
    std::cout << "\n";

    // Write the remaining data to output.
    std::cout << s.rdbuf();
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << "\n";
  }
}

class Client : public std::enable_shared_from_this<Client> {
  boost::asio::ip::tcp::socket m_Sock;
  char m_Buf[1024];
  char m_SendBuf[1024];

 public:
  Client(boost::asio::io_service& io) : m_Sock(io) {}

  boost::asio::ip::tcp::socket& sock() { return m_Sock; }

  void read() {
    m_Sock.async_read_some(
        boost::asio::buffer(m_Buf),
        boost::bind(&Client::handleRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  void handleRead(const boost::system::error_code& e,
                  std::size_t bytes_transferred) {
    if (e == boost::asio::error::eof) {
      std::cerr << "-client: " << m_Sock.remote_endpoint().address().to_string()
                << std::endl;
    }
    if (e)
      return;

    std::cerr << "read: " << bytes_transferred << " bytes" << std::endl;
    std::cerr << "read thread is " << pthread_self() << std::endl;

    int k = snprintf(m_SendBuf, sizeof(m_SendBuf), "HTTP/1.1 200 OK\r\n");
    k += snprintf(m_SendBuf + k, sizeof(m_SendBuf) - k,
                  "Content-Length: 26\r\n\r\n");
    k += snprintf(m_SendBuf + k, sizeof(m_SendBuf) - k, "ready proxy");

    m_Sock.async_write_some(
        boost::asio::buffer(m_SendBuf),
        [self = shared_from_this()](const boost::system::error_code& e,
                                    std::size_t bytes_transferred) -> void {
          // После того, как запишем ответ, можно снова читать
          self->read();
        });
  }
};

class Server {
 public:
  Server() : acceptor_(service_) {}

  void startServer() {
    int port = 5555;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen(1024);
    startAccept();

    std::cerr << "listen on port: " << port << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i)
      threads.push_back(std::thread(std::bind(&Server::run, this)));
    for (auto& thread : threads)
      thread.join();
    //      run();
  }

 private:
  void onAccept(std::shared_ptr<Client> c, const boost::system::error_code& e) {
    if (e)
      return;

    std::cerr << "+client: "
              << c->sock().remote_endpoint().address().to_string().c_str()
              << std::endl;

    c->read();
    startAccept();
  }

  void startAccept() {
    std::shared_ptr<Client> c(new Client(service_));
    acceptor_.async_accept(c->sock(),
                           boost::bind(&Server::onAccept, this, c,
                                       boost::asio::placeholders::error));
  }
  void run() { service_.run(); }

  boost::asio::io_service service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};

void func() {
  std::cerr << "func1" << std::endl;
}

struct Struct {
  void operator()() { std::cerr << "Struct::operator()" << std::endl; }
};

class Class : public std::enable_shared_from_this<Class> {
 public:
  void funcA(int arg) {
    std::cerr << "Class::func().int: " << arg << std::endl;
  }

  void funcB(int arg1, const std::string& arg2) {
    std::cerr << "Class::func(). arg1: " << arg1 << ", arg2: " << arg2
              << std::endl;
  }

  void funcC() { std::cerr << "Class::func2()" << std::endl; }
};

void queue_test() {
  boost::asio::io_service io;
  io.post(func);
  io.post([]() -> void { std::cerr << "lambda" << std::endl; });
  io.post(Struct());

  Class c;
  auto f1 = std::bind(&Class::funcC, &c);
  io.post(f1);

  std::shared_ptr<Class> c2 = std::make_shared<Class>();
  auto f2 = std::bind(&Class::funcC, c2->shared_from_this());
  io.post(f2);

  auto f3 = std::bind(&Class::funcA, c2->shared_from_this(), 10);
  io.post(f3);

  io.run_one();
  io.run_one();
  io.run_one();
  io.run_one();
  io.run_one();
  io.run_one();

  // io.run();
}

void bind_test() {
  Class c;
  auto f1 = std::bind(&Class::funcA, &c, std::placeholders::_1);
  auto f2 = std::bind(&Class::funcB, &c, std::placeholders::_1,
                      std::placeholders::_2);
  f1(123);
  f2(456, "abcd");

  std::shared_ptr<Class> c2 = std::make_shared<Class>();
  auto f3 = std::bind(&Class::funcB, c2->shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2);
  f3(789, "zxcv");

  auto f0 = std::bind(&Class::funcC, &c);
  f0();
}

int main(int argc, char* argv[]) {
  // sync()
  // sync_http();
  // queue_test();
  // sync_http_full("http://computing.kiae.ru");

  std::cout << "main thread is " << pthread_self() << std::endl;

  Server serv;
  serv.startServer();
  return 0;
}

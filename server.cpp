#include <arpa/inet.h>
#include <server.hpp>
#include <stdexcept>
#include <session.hpp>
#include <thread>
#include <cstring>

server::server(unsigned short port, const std::string& base_dir)
  : accept_sockfd_(socket(AF_INET, SOCK_STREAM, 0)),
    response_creator_(base_dir)
{
  if (accept_sockfd_ < 0)
    throw std::runtime_error(std::string("socket error ") + strerror(errno));

  sockaddr_in my_endpoint{};
  my_endpoint.sin_family = AF_INET;
  my_endpoint.sin_port = htons(port);
  my_endpoint.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(accept_sockfd_, (sockaddr*)&my_endpoint, sizeof(my_endpoint)))
    throw std::runtime_error(std::string("bind error ") + strerror(errno));
}

void server::run()
{
  if (listen(accept_sockfd_, 64) < 0)
    throw std::runtime_error(std::string("listen error ") + strerror(errno));
  for (;;)
  {
    int conn_sockfd = accept(accept_sockfd_, NULL, NULL);
    if (conn_sockfd < 0)
      throw std::runtime_error(std::string("accept error ") + strerror(errno));
    std::thread([conn_sockfd, this]
    {
      session(conn_sockfd, response_creator_).run();
    }).detach();
  }
}

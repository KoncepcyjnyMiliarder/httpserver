#include <session.hpp>
#include <stdexcept>
#include <request_parser.hpp>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/ip.h>

session::session(int sockfd, const response_maker& response_creator)
  : sockfd_(sockfd),
    response_creator_(response_creator)
{
}

session::~session()
{
  close(sockfd_);
}

void session::run()
{
  try
  {
    for (;;)
    {
      timeval time_limit{};
      time_limit.tv_sec = 1;
      time_limit.tv_usec = 0;
      fd_set sockfd_set;
      FD_ZERO(&sockfd_set);
      FD_SET(sockfd_, &sockfd_set);
      int ready = select(sockfd_ + 1, &sockfd_set, NULL, NULL, &time_limit);

      if (ready == 0)
        return;
      if (ready == -1)
        throw std::runtime_error(std::string("select error ") + strerror(errno));

      std::string recvbuf(IP_MAXPACKET, '\0');
      auto recvlen = recv(sockfd_, &recvbuf[0], recvbuf.size(), 0);
      if (recvlen < 0)
        throw std::runtime_error(std::string("recv error ") + strerror(errno));
      recvbuf.resize(recvlen);
      auto request = request_parser::parse_request(recvbuf);

      auto resp = response_creator_.make_response_for_request(request.request_path, request.host);
      send(sockfd_, &resp.front(), resp.size(), 0);
      if (request.keep_alive == false)
        return;
    }
  }
  catch (const request_parser::parsing_exception&)
  {
    auto resp = response_creator_.not_implemented_response();
    send(sockfd_, &resp.front(), resp.size(), 0);
  }
  catch (const std::exception&)
  {
    //just die
  }
}

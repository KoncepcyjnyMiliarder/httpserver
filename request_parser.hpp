#pragma once

#include <string>
#include <stdexcept>

namespace request_parser
{

class parsing_exception
  : public std::runtime_error
{
  public:
    parsing_exception(const std::string& msg)
      : std::runtime_error(msg)
    {}
};

struct http_get_request
{
  std::string request_path;
  std::string host;
  bool keep_alive;
};

http_get_request parse_request(const std::string& buffer);

}
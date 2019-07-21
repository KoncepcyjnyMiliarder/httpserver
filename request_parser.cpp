#include <request_parser.hpp>

namespace request_parser
{

http_get_request parse_request(const std::string& buffer)
{
  auto crlf_pos = buffer.find_first_of("\r\n");
  if (crlf_pos == std::string::npos)
    throw parsing_exception("Did not find first CRLF");
  auto first_space_pos = buffer.find(' ', 0);
  auto second_space_pos = buffer.find(' ', first_space_pos + 1);
  if(second_space_pos > crlf_pos)
    throw parsing_exception("First line malformed");
  if(buffer.substr(0, first_space_pos) != "GET")
    throw parsing_exception("First line is not GET");

  http_get_request request;
  request.request_path = buffer.substr(first_space_pos + 1, second_space_pos - first_space_pos - 1);
  request.keep_alive = (buffer.find("\r\nConnection: close\r\n", crlf_pos) == std::string::npos);
  auto host_pos = buffer.find("\r\nHost: ", crlf_pos);
  if (host_pos != std::string::npos)
  {
    auto host_end_pos = buffer.find("\r\n", host_pos + 2);
    request.host = buffer.substr(host_pos + 8, host_end_pos - host_pos - 8);
    auto port_semicolon = request.host.rfind(':');
    if (port_semicolon != std::string::npos)
      request.host.resize(port_semicolon);
  }
  return request;
}

}
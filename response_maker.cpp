#include <response_maker.hpp>
#include <fstream>
#include <stdexcept>

std::string response_maker::craft_header_and_content(const std::string& file,
    const std::string& filetype, unsigned reponse_code, const std::string& response_msg) const
{
  std::ifstream filein(base_dir_ + file, std::ios::binary);
  if (!filein)
    throw std::runtime_error(file + " - file is missing");
  filein.seekg(0, filein.end);
  std::string tempbuf(filein.tellg(), '\0');
  filein.seekg(0, filein.beg);
  filein.read(&tempbuf.front(), tempbuf.size());

  return "HTTP/1.1 " +
         std::to_string(reponse_code) +
         ' ' + response_msg + "\r\n"
         "Content-Type: " +
         (filetype_to_mime_.count(filetype) ? filetype_to_mime_.at(filetype) : filetype_to_mime_.at("unknown")) +
         "\r\n"
         "Content-Len: " +
         std::to_string(tempbuf.size()) +
         "\r\n\r\n" +
         tempbuf;
}

response_maker::response_maker(const std::string& base_dir)
  : base_dir_(base_dir),
    filetype_to_mime_
{
  { "txt", "text/html" },
  { "css", "text/css" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "jpeg", "image/jpeg" },
  { "png", "image/png" },
  { "pdf", "application/pdf" },
  { "unknown", "application/octet_stream" }
}
{
}

std::string response_maker::make_response_for_request(const std::string& path, const std::string& host) const
{
  if (path.empty() || path.find("..") != std::string::npos)
    return craft_header_and_content("403.txt", "html", 403, "Forbidden");
  try
  {
    if(path.back() == '/')
      return craft_header_and_content(host + path + "index.html", "html", 301, "Moved permanently");
    auto last_dot_pos = path.rfind('.');
    if(last_dot_pos == std::string::npos)
      return craft_header_and_content(host + path, "unknown", 200, "OK");
    return craft_header_and_content(host + path, path.substr(last_dot_pos + 1), 200, "OK");
  }
  catch (const std::runtime_error&)
  {
    return craft_header_and_content("404.txt", "html", 404, "Not found");
  }
  return not_implemented_response();
}

std::string response_maker::not_implemented_response() const
{
  return craft_header_and_content("501.txt", "html", 501, "Not implemented");
}

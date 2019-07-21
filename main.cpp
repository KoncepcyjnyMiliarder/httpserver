#include <iostream>
#include <server.hpp>

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " port directory\n";
    return 1;
  }
  unsigned short port = (unsigned short)strtoul(argv[1], nullptr, 10);
  if (port == 0)
  {
    std::cerr << "Improper port\n";
    return 1;
  }
  std::string dir_with_slash(argv[2]);
  if (dir_with_slash.back() != '/')
    dir_with_slash.push_back('/');
  try
  {
    server(port, dir_with_slash).run();
    return 0;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

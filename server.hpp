#pragma once

#include <response_maker.hpp>

class server
{
    const int accept_sockfd_;
    response_maker response_creator_;

  public:

    server(unsigned short port, const std::string& base_dir);
    void run();
};

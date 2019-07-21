#pragma once

#include <response_maker.hpp>

class session
{
    const int sockfd_;
    const response_maker& response_creator_;

  public:

    session(int sockfd, const response_maker& response_creator);
    ~session();
    void run();
};

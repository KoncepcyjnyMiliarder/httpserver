#pragma once

#include <string>
#include <unordered_map>

class response_maker
{
    const std::string base_dir_;
    const std::unordered_map<std::string, std::string> filetype_to_mime_;

    std::string craft_header_and_content(const std::string& file,
                                         const std::string& filetype, unsigned reponse_code, const std::string& response_msg) const;

  public:

    response_maker(const std::string& base_dir);
    std::string make_response_for_request(const std::string& path, const std::string& host) const;
    std::string not_implemented_response() const;
};
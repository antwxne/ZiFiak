/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_API_HPP
#define ZIA_API_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "IModule.hpp"
#include "HTTP/Response/Response.hpp"
#include "HTTP/Request/Request.hpp"

namespace API_n {

enum phase_e : char {
    PLOP,
    PLOUF
};

class API {
public:
    API() = default;
    ~API() = default;
    void loadModule(const std::string &name);
    std::shared_ptr<HTTP_n::Response> handleRequest(HTTP_n::Request &req);

private:
    std::unordered_map<phase_e, std::vector<std::shared_ptr<Module_n::IModule>>> _modules;
};
}
#endif //ZIA_API_HPP

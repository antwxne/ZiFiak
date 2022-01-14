/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_IMODULE_HPP
#define ZIA_IMODULE_HPP

#include <string>

#include "HTTP/Request/Request.hpp"
#include "HTTP/Response/Response.hpp"

namespace API_n {
enum phase_e : char;
}

namespace Module_n {

enum responseCode_e : char {
    OK,
    KO,
    CONTINUE
};

class IModule {
public:
    virtual API_n::phase_e getExecutionPhase() const noexcept = 0;
    virtual responseCode_e handleRequest(HTTP_n::Request &req, HTTP_n::Response &res) = 0;
};
}
#endif //ZIA_IMODULE_HPP

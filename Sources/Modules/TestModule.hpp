/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_TESTMODULE_HPP
#define ZIA_TESTMODULE_HPP

#include "AModule.hpp"

namespace Module_n {

class TestModule : public AModule{
public:
    TestModule(const Server_n::Configuration &configuration);
    responseCode_e handleRequest(HTTP_n::Request &req, HTTP_n::Response &res);
};
}
#endif //ZIA_TESTMODULE_HPP

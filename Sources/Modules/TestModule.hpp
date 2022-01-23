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
    TestModule(const JSONObject &configuration);
    responseCode_e handleRequest(IContext &context);
};
}
#endif //ZIA_TESTMODULE_HPP

/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "TestModule.hpp"
#include "Api.hpp"
#include "Debug/Debug.hpp"

Module_n::TestModule::TestModule(const JSONObject &configuration) : AModule(
    configuration)
{
    _phase = API_n::phase_e::CGI;
}

Module_n::responseCode_e Module_n::TestModule::handleRequest(IContext &context
)
{
    Debug::log("je suis une petite fleure");
    return Module_n::OK;
}

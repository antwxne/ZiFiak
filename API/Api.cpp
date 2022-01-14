/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "Api.hpp"

void API_n::API::loadModule(const std::string &name)
{
}

std::shared_ptr<HTTP_n::Response> API_n::API::handleRequest(
    HTTP_n::Request &req
)
{
    std::shared_ptr<HTTP_n::Response> newResponse = std::make_shared<HTTP_n::Response>();

    return newResponse;
}

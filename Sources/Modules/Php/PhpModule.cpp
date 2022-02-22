/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** PhpModule
*/

#include "Modules/Php/PhpModule.hpp"

//zia::modules::php::PhpCgi::PhpCgi() {}

void zia::modules::php::PhpCgi::Init(const ziapi::config::Node &cfg)
{

}

ziapi::Version zia::modules::php::PhpCgi::GetVersion() const noexcept 
{
    return {3, 1, 1};
}

ziapi::Version zia::modules::php::PhpCgi::GetCompatibleApiVersion() const noexcept
{
    return {3, 1, 1};
}

const char *zia::modules::php::PhpCgi::GetName() const noexcept
{
    return "Php cgi module";
}

const char *zia::modules::php::PhpCgi::GetDescription()() const noexcept
{
    return "Php cgi module for handle the php";
}

double zia::modules::php::PhpCgi::GetHandlerPriority() const
{
    return 0;
}

bool zia::modules::php::PhpCgi::ShouldHandle(const http::Context &ctx, const http::Request &req) const
{
    return true;
}

void zia::modules::php::PhpCgi::Handle(http::Context &ctx, const http::Request &req, http::Response &res)
{

}
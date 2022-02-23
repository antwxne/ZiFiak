/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** PhpModule
*/

#include "Modules/Php/PhpModule.hpp"


void zia::modules::php::PhpCgi::Init(const ziapi::config::Node &cfg)
{
    try {
        _env.push_back("SCRIPT_FILENAME=" + cfg["modules"]["PHP-CGI"]["path"].AsString());
    }
    catch (const std::exception& e) {
        _initSetUp = false;
    }
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

const char *zia::modules::php::PhpCgi::GetDescription() const noexcept
{
    return "Php cgi module for handle the php";
}

double zia::modules::php::PhpCgi::GetHandlerPriority() const noexcept
{
    return 0.5;
}

bool zia::modules::php::PhpCgi::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
{
    return _initSetUp;
}

void zia::modules::php::PhpCgi::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
{
    try {
        res.Bootstrap();
        _env.push_back("REDIRECT_STATUS=200");
        _env.push_back("REQUEST_METHOD=" + req.method);
        _env.push_back("SCRIPT_NAME=" + req.body);
        _env.push_back("PATH_INFO=/");
        _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
        _env.push_back("SERVER_NAME=Zifiak");
        _env.push_back("SERVER_PORT=");
        _env.push_back("SERVER_SOFWARE=");
        _env.push_back("REMOTE_IDENT=");
        _env.push_back("AUTH_TYPE=");
        _env.push_back("CONTENT_LENGHT=");
        _env.push_back("CONTENT_TYPE=");
        _env.push_back("GATEWAY_INTERFACE=");
        _env.push_back("PATH_TRANSLATED=");
        _env.push_back("QUERY_STRING=");
        _env.push_back("REMOTE_ADDR=");
        _env.push_back("REMOTE_USER=");
        //setenv("REDIRECT_STATUS", res.status_code, 1);
        //setenv("REQUEST_METHOD", req.method.c_str(), 1);
        //setenv("SCRIPT_NAME", req.body.c_str(), 1);
        //setenv("PATH_INFO", "/", 1);
        //setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
        //setenv("SERVER_NAME", "Zifiak", 1);
        //setenv("SERVER_PORT", "", 1);
        //setenv("SERVER_SOFTWARE", "", 1);
        //setenv("REMOTE_IDENT","", 1);
        //setenv("AUTH_TYPE", "", 1);
        //setenv("CONTENT_LENGTH", "", 1);
        //setenv("CONTENT_TYPE", "", 1);
        //setenv("GATEWAY_INTERFACE", "", 1);
        //setenv("PATH_TRANSLATED", "", 1);
        //setenv("QUERY_STRING","",1);
        //setenv("REMOTE_ADDR", "", 1);
        //setenv("REMOTE_USER", "", 1);
        std::system("./$SCRIPT_FILENAME > tmp");
        std::ifstream tmp("tmp");
        tmp.seekg(0, std::ios::end);
        size_t size = tmp.tellg();
        std::string buffer(size, ' ');
        tmp.seekg(0);
        tmp.read(&buffer[0], size);
        res.body = buffer;
    }
    catch (const std::exception& e) {
        res.status_code = ziapi::http::Code::kInternalServerError;
        res.reason = "Error in the path or with the cgi.";
    }
}
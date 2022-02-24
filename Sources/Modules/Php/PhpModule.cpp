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
        _env.push_back("SERVER_NAME=" + cfg["modules"]["PHP-CGI"]["server_name"].AsString());
        _env.push_back("SERVER_PORT=" + cfg["modules"]["PHP-CGI"]["server_port"].AsString());
        _env.push_back("SERVER_SOFWARE=" + cfg["modules"]["PHP-CGI"]["server_software"].AsString());
        _env.push_back("REMOTE_IDENT=" + cfg["modules"]["PHP-CGI"]["remote_ident"].AsString());
        _env.push_back("CONTENT_TYPE=" + cfg["modules"]["PHP-CGI"]["content_type"].AsString());
        _env.push_back("GATEWAY_INTERFACE=" + cfg["modules"]["PHP-CGI"]["gateway_interface"].AsString());
        _env.push_back("PATH_TRANSLATED=" + cfg["modules"]["PHP-CGI"]["path_translated"].AsString());
        _env.push_back("REMOTE_ADDR=" + cfg["modules"]["PHP-CGI"]["remote_addr"].AsString());
        _env.push_back("REMOTE_USER=" + cfg["modules"]["PHP-CGI"]["remote_user"].AsString());
        _env.push_back("PATH_INFO=" + cfg["modules"]["PHP-CGI"]["path"].AsString());
        _env.push_back("SERVER_PROTOCOL=" + cfg["modules"]["PHP-CGI"]["protocol"].AsString());
        _env.push_back("AUTH_TYPE=" + cfg["modules"]["PHP-CGI"]["auth_type"].AsString());
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
    std::string env = "";
    int i = 0;
    char buf[100];
    res.Bootstrap();
    res.body = "";

    try {
        _env.push_back("REDIRECT_STATUS=200");
        _env.push_back("REQUEST_METHOD=" + req.method);
        _env.push_back("SCRIPT_NAME=" + req.body);
        _env.push_back("CONTENT_LENGHT=");
        _env.push_back("QUERY_STRING=");

        while (i != _env.size()) {
            env += " " + _env[i];
            i++;
        }
        auto file = popen(("env -i " + env + "./$PATH_INFO").c_str(), 0);

        if (file != NULL) {
            while (std::fgets(buf, 100, file)) {
                res.body += buf;
            }
        }
        else {
            res.status_code = ziapi::http::Code::kInternalServerError;
            res.reason = "Error in the path or with the cgi.";    
        }
    }
    catch (const std::exception& e) {
        res.status_code = ziapi::http::Code::kInternalServerError;
        res.reason = "Error in the path or with the cgi.";
    }
}
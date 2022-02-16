/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** HttpModule
*/

#include "HttpModule.hpp"

namespace zia::modules::http {

HttpModule::HttpModule()
{
}

HttpModule::~HttpModule()
{
}

void getHeaderBody(ziapi::http::Request &req, std::stringstream &stream)
{
    
}

std::string &getTarget(std::stringstream &stream)
{
    std::string str;
    std::getline(stream, str, ' ');
    if (str.empty())
        throw std::invalid_argument("Http request Usage: {Version} {Method} {target} {header} {body}");
    return str;
}

std::string &getMethod(std::stringstream &stream)
{
    std::string method;
    std::getline(stream, method, ' ');
    if (std::find(_methods.begin(), _methods.end(), method) == _methods.end())
        throw std::invalid_argument("Http request Usage: {Version} {Method} {target} {header} {body}");
    return method;
}

ziapi::http::Version getVersion(std::stringstream &stream)
{
    std::string version;
    std::getline(stream, version, ' ');
    if (_versions.find(version) == _versions.end())
        throw std::invalid_argument("Http request Usage: {Version} {Method} {target} {header} {body}");
    return _versions.at(version);
}

ziapi::http::Request &HttpModule::createRequest(std::string &str)
{
    //ex of string : KV1 GET target Content-Type: text/html Content-length: 345 body
    ziapi::http::Request req;
    std::stringstream stream(str);

    try {
        req.version = getVersion(stream);
        req.method = getMethod(stream);
        req.target = getTarget(stream);
        getHeaderBody(req, stream);
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }
    return req;
}

std::string &HttpModule::readResponse(ziapi::http::Response &res)
{
}

}

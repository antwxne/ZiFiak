/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** HttpModule
*/

#include "HttpModule.hpp"

namespace zia::modules::http {

void getHeaderBody(ziapi::http::Request &req, std::stringstream &stream)
{
    std::string str;
    std::string temp;
    std::string mod;

    while (std::getline(stream, str, ' ')) {
        mod = str;
        mod.resize(mod.size()-1);
        if (std::find(_headers.begin(), _headers.end(), mod) != _headers.end()) {
            std::getline(stream, temp, ' ');
            req.headers[mod] = temp;
        } else
            req.body += ' ' + str;
    }
}

std::string getTarget(std::stringstream &stream)
{
    std::string str;

    std::getline(stream, str, ' ');
    if (str.empty())
        throw std::invalid_argument("getTarget - Http request Usage: {Version} {Method} {target} {header} {body}");
    return str;
}

std::string getMethod(std::stringstream &stream)
{
    std::string method;

    std::getline(stream, method, ' ');
    if (std::find(_methods.begin(), _methods.end(), method) == _methods.end())
        throw std::invalid_argument("getMethod - Http request Usage: {Version} {Method} {target} {header} {body}");
    return method;
}

ziapi::http::Version getVersion(std::stringstream &stream)
{
    std::string version;

    std::getline(stream, version, ' ');
    if (_versions.find(version) == _versions.end())
        throw std::invalid_argument("getVersion - Http request Usage: {Version} {Method} {target} {header} {body}");
    return _versions.at(version);
}

std::string clear_string(const std::string &str)
{
    std::string cleared;
    std::string temp = str;
    std::regex spaces(R"(\s+)");

    temp = std::regex_replace(temp, spaces, " ");
    while (temp[0] && temp[0] == ' ')
        temp.erase(0, 1);
    while (temp[0] != '\0') {
        while (temp[0] == ' ' && temp[1] && temp[1] == ' ')
            temp.erase(0, 1);
        if (temp[0] == ' ' && temp[1] == '\0')
            return cleared;
        cleared += temp[0];
        temp.erase(0, 1);
    }
    return cleared;
}

ziapi::http::Request HttpModule::createRequest(const std::string &str)
{
    //ex of string : KV1 GET target Content-Type: text/html Content-length: 345 body
    ziapi::http::Request req;
    std::string temp = clear_string(str);
    std::stringstream stream(temp);

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

std::string HttpModule::readResponse(const ziapi::http::Response &res) noexcept
{
    std::string str;

    for (const auto &v : _versions) {
        if (v.second == res.version){
            str += v.first;
            break;
        }
    }
    str += ' ' + _codes.at(res.status_code);
    str += ' ' + res.reason;
    for (const auto &f : res.headers)
        str += ' ' + f.first + ": " + f.second;
    str += ' ' + res.body;
    return str;
}

}

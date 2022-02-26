/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** HttpModule
*/

#include "HTTPParser.hpp"
#include "Debug/Debug.hpp"

namespace zia::modules::network {

void getHeaderBody(ziapi::http::Request &req, std::stringstream &stream)
{
    std::string str;
    std::string temp;
    std::string mod;

    while (std::getline(stream, str, ' ')) {
        mod = str;
        mod.resize(mod.size() - 1);
        if (std::find(_headers.begin(), _headers.end(), mod) !=
            _headers.end()) {
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
        throw std::invalid_argument(
            "getTarget - Http request Usage: {Version} {Method} {target} {header} {body}");
    return str;
}

std::string getMethod(std::stringstream &stream)
{
    std::string method;

    std::getline(stream, method, ' ');
    if (std::find(_methods.begin(), _methods.end(), method) == _methods.end())
        throw std::invalid_argument(
            "getMethod - Http request Usage: {Version} {Method} {target} {header} {body}");
    return method;
}

ziapi::http::Version getVersion(std::stringstream &stream)
{
    std::string version;

    std::getline(stream, version, ' ');
    if (_versions.find(version) == _versions.end())
        throw std::invalid_argument(
            "getVersion - Http request Usage: {Version} {Method} {target} {header} {body}");
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

ziapi::http::Request HTTPParser::createRequest(const std::string &str)
{
    //ex of string : KV1 GET target Content-Type: text/html Content-length: 345 body
    ziapi::http::Request req;
    std::string temp = clear_string(str);
    std::stringstream stream(temp);

    Debug::log("client buffer: " + str);
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

std::string HTTPParser::readResponse(const ziapi::http::Response &res) noexcept
{
    std::string str;

    for (const auto &v: _versions) {
        if (v.second == res.version) {
            str += v.first;
            break;
        }
    }
    str += ' ' + _codes.at(res.status_code);
    str += ' ' + res.reason;
    for (const auto &f: res.headers)
        str += ' ' + f.first + ": " + f.second;
    str += ' ' + res.body;
    return str;
}

bool HTTPParser::isRequestComplete(const ziapi::http::Request &req) noexcept
{
    if (req.method != ziapi::http::method::kPatch &&
        req.method != ziapi::http::method::kPost &&
        req.method != ziapi::http::method::kPut) {
        return true;
    }
    int contentLength;
    try {
        contentLength = std::atoi(
            req.headers.at(ziapi::http::header::kContentLength).c_str());
    } catch (const std::out_of_range &error) {
        Debug::warn(error.what());
        return true;
    }
    if (contentLength != 0) {
        return contentLength == req.body.size();
    } else {
        return true;
    }
}

std::pair<int, int> HTTPParser::parseKeepAliveInfos(const std::string &value)
{
    auto replace_char = [](std::string &str, const std::string &substr, char c) {
        for (auto &i: str) {
            for (const auto &a: substr) {
                if (i == a) {
                    i = c;
                }
            }
        }
    };
    std::string cpyValue(value);
    char *timeout = std::strstr(&*cpyValue.begin(), "timeout=") + std::strlen("timeout=");
    char *max = std::strstr(&*cpyValue.begin(), "max=") + std::strlen("max=");
    std::pair<int, int> dest;

    replace_char(cpyValue, ", ", '\0');
    dest.first = std::atoi(timeout);
    dest.second = std::atoi(max);
    return dest;
}
}

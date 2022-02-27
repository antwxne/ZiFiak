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

    std::getline(stream, str);
    while (std::getline(stream, str, '\r')) {
        std::stringstream temp_stream(str);
        std::getline(temp_stream, mod, ' ');
        mod.resize(mod.size() - 1);
        if (std::find(_headers.begin(), _headers.end(), mod) !=
            _headers.end()) {
            std::getline(temp_stream, temp);
            req.headers[mod] = temp;
        } else {
            req.body += ' ' + str;
        }
        std::getline(stream, str);
    }
}

std::string getTarget(std::string &target)
{
    if (target.empty())
        throw std::invalid_argument(
            "getTarget - Http request Usage:\n{Method}\\r\\n{target}\\r\\n{version}\\r\\n{header}\\r\\n{body}");
    return target;
}

std::string getMethod(std::string &method)
{
    if (std::find(_methods.begin(), _methods.end(), method) == _methods.end())
        throw std::invalid_argument(
            "getMethod - Http request Usage:\n{Method}\\r\\n{target}\\r\\n{version}\\r\\n{header}\\r\\n{body}");
    return method;
}

ziapi::http::Version getVersion(std::string &version)
{
    if (_versions.find(version) == _versions.end())
        throw std::invalid_argument(
            "getVersion - Http request Usage:\n{Method}\\r\\n{target}\\r\\n{version}\\r\\n{header}\\r\\n{body}");
    return _versions.at(version);
}

std::string clear_string(const std::string &str)
{
    std::string cleared;
    std::string temp = str;
    std::regex spaces("( +)");

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
    ziapi::http::Request req;
    std::stringstream stream(clear_string(str));
    std::string temp;

    Debug::log("client buffer: " + str);
    try {
        std::getline(stream, temp, ' ');
        req.method = getMethod(temp);
        std::getline(stream, temp, ' ');
        req.target = getTarget(temp);
        std::getline(stream, temp, '\r');
        req.version = getVersion(temp);
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

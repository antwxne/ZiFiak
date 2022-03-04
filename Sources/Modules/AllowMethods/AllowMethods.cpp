/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** AllowMethods
*/

#include <algorithm>
#include <exception>
#include "Debug/Debug.hpp"
#include "ziapi/HttpConstants.hpp"
#include "AllowMethods.hpp"

AllowMethods::AllowMethods()
{
}

void AllowMethods::Init(const ziapi::config::Node &config)
{
    try {
        for (const auto &elem : config["AllowMethods"].AsArray()) {
            auto obj = elem->AsDict();
            std::string path = obj["path"]->AsString();
            std::vector<std::string> methods;
            for (const auto &e : obj["methods"]->AsArray()) {
                methods.push_back(e->AsString());
            }
            this->_authorized.push_back({path, methods});
            // std::string method(elem->AsString());
            // std::transform(method.begin(), method.end(), method.begin(), [](uint8_t c){ return std::toupper(c); });
            // this->_authorized.push_back(method);
        }
    }
    catch(const std::exception& e)
    {
        Debug::warn("Failed to init AllowMethods module");
        this->_authorized.push_back({"/", {"reset"}});
    }
}

ziapi::Version AllowMethods::GetVersion() const noexcept
{
    return {4, 0, 0};
}

ziapi::Version AllowMethods::GetCompatibleApiVersion() const noexcept
{
    return {4, 0, 0};
}

const char *AllowMethods::GetName() const noexcept
{
    return "AllowMethods";
}

const char *AllowMethods::GetDescription() const noexcept
{
    return "Restreint l'accès aux méthodes HTTP spécifiées";
}

void AllowMethods::PreProcess(ziapi::http::Context &context, ziapi::http::Request &req)
{    
    // throw std::runtime_error("");throw Method not allowed on this server + status code 405
    context.insert({"ErrorOccured", std::pair(ziapi::http::Code::kMethodNotAllowed, ziapi::http::reason::kMethodNotAllowed)});
}

double AllowMethods::GetPreProcessorPriority() const noexcept
{
    return 0.0f;
}

bool AllowMethods::ShouldPreProcess(const ziapi::http::Context &context, const ziapi::http::Request &req) const
{
    std::pair<std::string, std::vector<std::string>> getDirOptions({"", {""}});

    for (const auto &e : this->_authorized)
        if (req.target.find(e.first) == 0 && e.first.size() > getDirOptions.first.size())
            getDirOptions = e;

    if (getDirOptions.second[0] == "reset")
        return false;
    for (const auto &e : getDirOptions.second) {
        if (e == req.method) {
            return false;
        }
    }
    return true;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new AllowMethods;
}
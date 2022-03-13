/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Environment
*/

#include <sstream>
#include "Environment.hpp"
#include "Debug/Debug.hpp"

Environment::Environment()
{
}

void Environment::Init(const ziapi::config::Node &config)
{
}

[[nodiscard]] ziapi::Version Environment::GetVersion() const noexcept
{
    return {5, 0, 0};
}

[[nodiscard]] ziapi::Version Environment::GetCompatibleApiVersion() const noexcept
{
    return {5, 0, 1};
}

[[nodiscard]] const char *Environment::GetName() const noexcept
{
    return "Environment";
}

[[nodiscard]] const char *Environment::GetDescription() const noexcept
{
    return "Module pour ajouter/supprimer des variables d'environnement pour les handlers.";
}

void Environment::PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &req)
{
    std::vector<std::pair<std::string, std::string>> change;
    std::stringstream stream(req.body);
    std::string tmp;
    std::string separator = "%3D";

    std::cout << req.body << std::endl;
    while (std::getline(stream, tmp, '&')) {
        for (auto &it : _commands) {
            if (!tmp.compare(0, it.length(), it)) {
                if (tmp.find("%3D") != tmp.npos) {
                    auto equalBegin = tmp.find(separator);
                    change.push_back({it.substr(0, it.length() - 1), tmp.substr(it.length(), equalBegin - it.length()) + "=" + tmp.substr(equalBegin + separator.length())});
                } else {
                    change.push_back({it.substr(0, it.length() - 1), tmp.substr(it.length())});
                }
            }
        }
    }
    for (auto &it : change) {
        ctx.insert({it.first, it.second});
    }
}

[[nodiscard]] double Environment::GetPreProcessorPriority() const noexcept
{
    return 1.0f;
}

[[nodiscard]] bool Environment::ShouldPreProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
{
    return true;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new Environment;
}
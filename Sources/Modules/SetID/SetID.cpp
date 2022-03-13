/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** SetID
*/

#include <sstream>
#include <cstring>
#include <stdio.h>
#include "SetID.hpp"
#include "Debug/Debug.hpp"

SetID::SetID() : _active(false), _id(0)
{
}

void SetID::Init(const ziapi::config::Node &config)
{
    try {
        this->_active = config["SetID"]["activated"].AsBool();
    } catch (const std::exception& e) {
        Debug::warn("Failed to init SetID module");
    }
}

[[nodiscard]] ziapi::Version SetID::GetVersion() const noexcept
{
    return {5, 0, 0};
}

[[nodiscard]] ziapi::Version SetID::GetCompatibleApiVersion() const noexcept
{
    return {5, 0, 1};
}

[[nodiscard]] const char *SetID::GetName() const noexcept
{
    return "SetID";
}

[[nodiscard]] const char *SetID::GetDescription() const noexcept
{
    return "Module pour ajouter un unique ID pour chaque requetes fait sur le server";
}

void SetID::PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &req)
{
    auto port = std::any_cast<std::uint16_t>(ctx.at("client.socket.port"));
    auto address = std::any_cast<std::string>(ctx.at("client.socket.address"));
    time_t now = time(0);
    std::size_t tmp;

    while ((tmp = address.find('.')) != address.npos) {
        address.erase(tmp, 1);
    }
    req.headers.insert({"unique_id", std::to_string(now) + address + std::to_string(port) + std::to_string(_id)});
    _id += 1;
}

[[nodiscard]] double SetID::GetPreProcessorPriority() const noexcept
{
    return 0.0f;
}

[[nodiscard]] bool SetID::ShouldPreProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
{
    return this->_active;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new SetID;
}
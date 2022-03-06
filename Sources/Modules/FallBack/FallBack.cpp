/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** FallBack
*/

#include "FallBack.hpp"

FallBack::FallBack()
{
}

FallBack::~FallBack()
{
}

void FallBack::Init([[maybe_unused]] const ziapi::config::Node &)
{
    // Don't need anything to configure in this implementation
}

[[nodiscard]] ziapi::Version FallBack::GetVersion() const noexcept
{
    return ziapi::Version{5, 0, 0};
}

[[nodiscard]] ziapi::Version FallBack::GetCompatibleApiVersion() const noexcept
{
    return ziapi::Version{5, 0, 1};
}

[[nodiscard]] const char *FallBack::GetName() const noexcept
{
    return "Compressor Module";
}

[[nodiscard]] const char *FallBack::GetDescription() const noexcept
{
    return "Check if an error occured during pipelines and set error response properly";
}

void FallBack::PostProcess(ziapi::http::Context &context, const ziapi::http::Request &, ziapi::http::Response &res)
{
    auto errorOccured = std::any_cast<std::pair<ziapi::http::Code,const char *>>(context["ErrorOccured"]);
    res.headers.clear();
    res.status_code = errorOccured.first;
    res.reason = errorOccured.second;
    res.body = res.reason;
    res.headers.insert({"Content-Length", std::to_string(res.body.size())});
}

[[nodiscard]] double FallBack::GetPostProcessorPriority() const noexcept
{
    return 0.0f;
}

bool FallBack::ShouldPostProcess(const ziapi::http::Context &context, const ziapi::http::Request &req, const ziapi::http::Response &res) const
{
    if (context.find("ErrorOccured") != context.end())
        return true;
    return false;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new FallBack;
}

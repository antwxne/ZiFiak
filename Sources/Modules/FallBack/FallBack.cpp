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

ziapi::Version FallBack::GetVersion() const
{
    return ziapi::Version{3, 0, 0};
}

ziapi::Version FallBack::GetCompatibleApiVersion() const
{
    return ziapi::Version{3, 0, 0};
}

const char *FallBack::GetName() const
{
    return "CompressorModule";
}

const char *FallBack::GetDescription() const
{
    return "Compress the response body before sending it back to the network";
}

void PostProcess(ziapi::http::Context &context, ziapi::http::Response &res)
{
    auto errorOccured = std::any_cast<std::pair<ziapi::http::Code, std::string>>(context["ErrorOccured"]);
    res.Bootstrap(errorOccured.first, errorOccured.second);
}


double FallBack::GetPostProcessorPriority() const
{
    return 0.0f;
}

bool FallBack::ShouldPostProcess(const ziapi::http::Context &context, const ziapi::http::Response &res) const
{
    if (context.find("ErrorOccured") != context.end())
        return true;
    return false;
}

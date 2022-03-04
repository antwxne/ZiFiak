/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Log
*/

#include <ostream>
#include <fstream>
#include "Debug/Debug.hpp"
#include "Log.hpp"

Log::Log() : _setupped(false)
{
}

void Log::Init(const ziapi::config::Node &config)
{
    try {
        this->_setupped = config["Log"]["activated"].AsBool();
        this->_filePath = config["Log"]["path"].AsString();
    } catch(const std::exception& e) {
        Debug::warn("Failed to init Log module");
        this->_setupped = false;
    }
    
}

ziapi::Version Log::GetVersion() const noexcept
{
    return {5, 0, 0};
}

ziapi::Version Log::GetCompatibleApiVersion() const noexcept
{
    return {5, 0, 0};
}

const char *Log::GetName() const noexcept
{
    return "Log";
}

const char *Log::GetDescription() const noexcept
{
    return "Module pour log les requetes faites sur le server";
}

void Log::PostProcess(ziapi::http::Context &context, const ziapi::http::Request &req, ziapi::http::Response &res)
{
    std::ofstream outfile(this->_filePath, std::ofstream::out | std::ofstream::app);
    std::string outString;

    if (!outfile.is_open()) {
        return;
    }
    outString = "[" + req.method + " " + req.target + "] " + std::to_string(static_cast<int>(res.status_code)) + ": " + res.reason;
    outfile << outString << std::endl;
}

double Log::GetPostProcessorPriority() const noexcept
{
    return 0.0f;
}

bool Log::ShouldPostProcess(const ziapi::http::Context &context, const ziapi::http::Request &req, const ziapi::http::Response &res) const
{
    return this->_setupped;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new Log;
}

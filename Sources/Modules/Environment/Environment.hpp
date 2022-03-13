/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Environment
*/

#pragma once
#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

static const std::vector<std::string> _commands = {"setenv=", "unsetenv="};

class Environment : public ziapi::IPreProcessorModule {
    public:
        Environment();
        ~Environment() = default;

        void Init(const ziapi::config::Node &) override;

        [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

        [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

        [[nodiscard]] const char *GetName() const noexcept override;

        [[nodiscard]] const char *GetDescription() const noexcept override;

        void PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &req) override;

        [[nodiscard]] double GetPreProcessorPriority() const noexcept override;

        [[nodiscard]] bool ShouldPreProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const override;

    private:
        bool _active;
};

DYLIB_API ziapi::IModule *LoadZiaModule();
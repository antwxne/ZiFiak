/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** PhpModule
*/

#pragma once

#include <stdlib.h>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "ziapi/Module.hpp"

namespace zia::modules::php {

class PhpCgi : public ziapi::IHandlerModule {
    public:
        PhpCgi() = default;
        ~PhpCgi() = default;

        //IModule
        void Init(const ziapi::config::Node &cfg) override;
        ziapi::Version GetVersion() const noexcept override;
        ziapi::Version GetCompatibleApiVersion() const noexcept override;
        const char *GetName() const noexcept override;
        const char *GetDescription() const noexcept override;

        //IHandlerModule
        double GetHandlerPriority() const noexcept override;
        bool ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const override;
        void Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;

        void EnvSetUp(const ziapi::http::Request &req) noexcept;

    private:
            bool _initSetUp = true;
            std::vector<std::string> _env;
    protected:

};
}
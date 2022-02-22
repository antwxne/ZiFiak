/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** PhpModule
*/

#pragma once

#include "ziapi/Module.hpp"

namespace zia::modules::php {

class PhpCgi : public ziapi::IHandlerModule {
    public:
        PhpCgi() = default;
        ~PhpCgi() = default;

        //IModule
        void Init(const config::Node &cfg) override; // Set up environnement variables / get the config values
        ziapi::Version GetVersion() const noexcept override;
        ziapi::Version GetCompatibleApiVersion() const noexcept override;
        const char *GetName() const noexcept override;
        const char *GetDescription() const noexcept override;

        //IHandlerModule
        double GetHandlerPriority() const override;
        bool ShouldHandle(const http::Context &ctx, const http::Request &req) const override;
        void Handle(http::Context &ctx, const http::Request &req, http::Response &res) override; // execute the php with php-cgi

    private:

    protected:

};
}
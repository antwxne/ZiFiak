/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** FallBack
*/

#ifndef FALLBACK_HPP_
#define FALLBACK_HPP_

#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

class FallBack : public ziapi::IPostProcessorModule
{
    public:
        FallBack();
        ~FallBack();

    void Init(const ziapi::config::Node &) override;

    [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

    [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

    [[nodiscard]] const char *GetName() const noexcept override;

    [[nodiscard]] const char *GetDescription() const noexcept override;

    void PostProcess(ziapi::http::Context &, const ziapi::http::Request &, ziapi::http::Response &res) override;

    [[nodiscard]] double GetPostProcessorPriority() const noexcept override;

    [[nodiscard]] bool ShouldPostProcess(const ziapi::http::Context &, const ziapi::http::Request &, const ziapi::http::Response &) const override;

    protected:
    private:
};

DYLIB_API ziapi::IModule *LoadZiaModule();

#endif /* !FALLBACK_HPP_ */
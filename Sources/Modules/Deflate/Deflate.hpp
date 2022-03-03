/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Deflate
*/

#ifndef DEFLATE_HPP_
#define DEFLATE_HPP_

#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

class Deflate : public ziapi::IPostProcessorModule
{
    public:
        Deflate();
        ~Deflate();

        void Init(const ziapi::config::Node &) override;

    [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

    [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

    [[nodiscard]] const char *GetName() const noexcept override;

    [[nodiscard]] const char *GetDescription() const noexcept override;

    void PostProcess(ziapi::http::Context &, const ziapi::http::Request &,ziapi::http::Response &res) override;

    [[nodiscard]] double GetPostProcessorPriority() const noexcept override;

    [[nodiscard]] bool ShouldPostProcess(const ziapi::http::Context &, const ziapi::http::Request &, const ziapi::http::Response &) const override;

        std::string _uncompressString(const std::string &str) const noexcept;
    protected:
    private:
        std::string _compressString(const std::string &str) const noexcept;
        bool _activated;
};

DYLIB_API ziapi::IModule *LoadZiaModule();

#endif /* !DEFLATE_HPP_ */

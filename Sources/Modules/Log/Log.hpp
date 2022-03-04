/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Log
*/

#ifndef LOG_HPP_
#define LOG_HPP_

#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

class Log : public ziapi::IPostProcessorModule
{
    public:
        Log();
        ~Log() = default;

        void Init(const ziapi::config::Node &) override;

        [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

        [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

        [[nodiscard]] const char *GetName() const noexcept override;

        [[nodiscard]] const char *GetDescription() const noexcept override;

        void PostProcess(ziapi::http::Context &, const ziapi::http::Request &req, ziapi::http::Response &res) override;

        [[nodiscard]] double GetPostProcessorPriority() const noexcept override;

        [[nodiscard]] bool ShouldPostProcess(const ziapi::http::Context &, const ziapi::http::Request &, const ziapi::http::Response &) const override;
    protected:
    private:
        bool _setupped;
        std::string _filePath;
};

DYLIB_API ziapi::IModule *LoadZiaModule();

#endif /* !LOG_HPP_ */

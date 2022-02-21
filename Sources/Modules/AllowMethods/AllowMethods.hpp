/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** AllowMethods
*/

#ifndef ALLOWMETHODS_HPP_
#define ALLOWMETHODS_HPP_

#include <vector>
#include "ziapi/Module.hpp"

class AllowMethods : public ziapi::IPreProcessorModule
{
    public:
        AllowMethods();
        ~AllowMethods() = default;

        void Init(const ziapi::config::Node &) override;
        [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

        [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

        [[nodiscard]] const char *GetName() const noexcept override;

        [[nodiscard]] const char *GetDescription() const noexcept override;

        void PreProcess(ziapi::http::Context &, ziapi::http::Request &req) override;

        [[nodiscard]] double GetPreProcessorPriority() const noexcept override;

        [[nodiscard]] bool ShouldPreProcess(const ziapi::http::Context &, const ziapi::http::Request &req) const override;

    protected:
    private:
        std::vector<std::pair<std::string, std::vector<std::string>>> _authorized;
};

#endif /* !ALLOWMETHODS_HPP_ */
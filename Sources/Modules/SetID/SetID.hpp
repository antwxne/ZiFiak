/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** SetID
*/

#pragma once
#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

class SetID : public ziapi::IPreProcessorModule {
    public:
        SetID();
        ~SetID() = default;

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
        std::size_t _id;
};

struct __attribute__((packed)) uniqueId {
    u_int64_t time;
    u_int32_t ip;
    u_int16_t port;
    u_int8_t id;
};

DYLIB_API ziapi::IModule *LoadZiaModule();
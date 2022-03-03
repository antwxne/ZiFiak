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
#include "dylib/dylib.hpp"
#include <stdio.h>
#include "ziapi/Module.hpp"

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>
#define BUFSIZE 4096

#endif


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

#if defined(_WIN32) || defined(_WIN64)

        void WriteToPipe() noexcept;
        std::string GetFromPipe() noexcept;
        void CreateChildProcess(std::string env, std::string exec);

#endif

    private:
            bool _initSetUp = true;
            std::vector<std::string> _env;
            std::string _cgi;
            std::vector<std::pair<std::string, std::string>> _paths;

#if defined(_WIN32) || defined(_WIN64)

            HANDLE g_hChildStd_IN_Rd = nullptr;
            HANDLE g_hChildStd_IN_Wr = nullptr;
            HANDLE g_hChildStd_OUT_Rd = nullptr;
            HANDLE g_hChildStd_OUT_Wr = nullptr;
            HANDLE g_hInputFile = nullptr;

#endif

};
}
DYLIB_API ziapi::IModule *LoadZiaModule();
/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** PhpModule
*/

#include "Modules/Php/PhpModule.hpp"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void zia::modules::php::PhpCgi::Init(const ziapi::config::Node &cfg)
{
    try {
        _cgi = cfg["modules"]["PHP-CGI"]["full_path"].AsString();
        for (const auto& e : cfg["modules"]["PHP-CGI"]["script_filename"].AsArray()) {
            auto obj = e->AsDict();
            _paths.push_back({obj["url"]->AsString(),obj["filePath"]->AsString()});
        }
        _env.push_back("CONTENT_TYPE=" + cfg["modules"]["PHP-CGI"]["content_type"].AsString());
        _env.push_back("GATEWAY_INTERFACE=" + cfg["modules"]["PHP-CGI"]["gateway_interface"].AsString());
        _env.push_back("PATH_INFO=" + cfg["modules"]["PHP-CGI"]["path_info"].AsString());
        _env.push_back("PATH_TRANSLATED=" + cfg["modules"]["PHP-CGI"]["path_translated"].AsString());
        _env.push_back("REMOTE_IDENT=" + cfg["modules"]["PHP-CGI"]["remote_ident"].AsString());
        _env.push_back("REMOTE_USER=" + cfg["modules"]["PHP-CGI"]["remote_user"].AsString());
        _env.push_back("SERVER_NAME=" + cfg["modules"]["PHP-CGI"]["server_name"].AsString());
        _env.push_back("SERVER_PORT=" + cfg["modules"]["PHP-CGI"]["server_port"].AsString());
        _env.push_back("SERVER_PROTOCOL=" + cfg["modules"]["PHP-CGI"]["protocol"].AsString());
        _env.push_back("SERVER_SOFTWARE=" + cfg["modules"]["PHP-CGI"]["server_software"].AsString());
        _env.push_back("REMOTE_HOST=" + cfg["modules"]["PHP-CGI"]["remote_host"].AsString());
        _env.push_back("REMOTE_ADDR=" + cfg["modules"]["PHP-CGI"]["ip_client"].AsString());
        _env.push_back("AUTH_TYPE=" + cfg["modules"]["PHP-CGI"]["auth_type"].AsString());
        _env.push_back("REDIRECT_STATUS=200");
    }
    catch (const std::exception& e) {
        _initSetUp = false;
    }
}

ziapi::Version zia::modules::php::PhpCgi::GetVersion() const noexcept 
{
    return {3, 1, 1};
}

ziapi::Version zia::modules::php::PhpCgi::GetCompatibleApiVersion() const noexcept
{
    return {3, 1, 1};
}

const char *zia::modules::php::PhpCgi::GetName() const noexcept
{
    return "Php cgi module";
}

const char *zia::modules::php::PhpCgi::GetDescription() const noexcept
{
    return "Php cgi module for handle the php";
}

double zia::modules::php::PhpCgi::GetHandlerPriority() const noexcept
{
    return 0.5;
}

bool zia::modules::php::PhpCgi::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
{
    for (auto elem : _paths) {
        if (req.target.find(elem.first) == 0) {
            return _initSetUp;
        }
    }
    return false;
}

void zia::modules::php::PhpCgi::EnvSetUp(const ziapi::http::Request &req) noexcept
{
        for (auto elem : _paths) {
            if (req.target.find(elem.first) == 0) {
                _env.push_back("SCRIPT_FILENAME=" + elem.second);
                _env.push_back("SCRIPT_NAME=" + elem.second);
            std::cout << "ENV: " << elem.second << std::endl;
                break;
            }
        }
        _env.push_back("REQUEST_METHOD=" + req.method);
        _env.push_back("QUERY_STRING=");
        _env.push_back("CONTENT_LENGTH=" + std::to_string(req.body.length()));
        _env.push_back("HTTP_ACCEPT=*/*");
        _env.push_back("HTTP_ACCEPT_LANGUAGE=fr");
        _env.push_back("HTTP_USER_AGENT=charlie");
        _env.push_back("HTTP_COOKIE=");
}

#if defined(_WIN32) || defined(_WIN64)
void zia::modules::php::PhpCgi::WriteToPipe()
{
    DWORD dwRead;
    DWORD dwWritten;
    char chBuf[BUFSIZE];
    bool bSuccess = false;

    while (1) {
        bSuccess = ReadFile(g_hInputFile, chBuf, BUFSIZE, &dwRead, nullptr);
        if (!bSuccess || dwRead == 0) {
            break;
        }
        bSuccess = WriteFile(g_hChildStd_IN_Wr, chBuf, dwRead, &dwWritten, nullptr);
        if (!bSuccess) {
            break;
        }
    }
    CloseHandle(g_hChildStd_IN_Wr);
}

std::string zia::modules::php::PhpCgi::GetFromPipe()
{
    int i = 0;
    DWORD dwRead;
    DWORD dwWritten;
    char chBuf[BUFSIZE];
    bool bSuccess = false;
    std::string tmp = {};

    while (1) {
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        while (i != dwRead) {
            tmp += chBuf[i];
            i++;
        }
        i = 0;
        if (!bSuccess || dwRead == 0) {
            break;
        }
    }
    return (tmp);
}


void zia::modules::php::PhpCgi::CreateChildProcess(std::string env, std::string exec)
{
    TCHAR szCmdline[] = TEXT("child");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    bool bSuccess = false;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));

    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags = STARTF_USESTDHANDLES;

    bSuccess = CreateProcessA(nullptr, _strdup(exec.c_str()), nullptr, nullptr, true, 0, (LPVOID)env.data(), nullptr, &siStartInfo, &piProcInfo);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(g_hChildStd_OUT_Wr);
    CloseHandle(g_hChildStd_IN_Rd);
}
#endif


void zia::modules::php::PhpCgi::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
{
    int i = 0;
    int pos = 0;
    int tokenPos = 0;
    FILE *file;
    std::array<char, 128> buf;
    std::string env = {};
    std::string resp = {};
    std::string token;
    std::string lilToken;

    EnvSetUp(req);

#if defined(_WIN32) || defined(_WIN64)

    std::string exec = _cgi;

    SECURITY_ATTRIBUTES saAttr;

    while (i != _env.size()) {
        env += {(_env[i] + '\0')};
        i++;
    }
    env += '\0';

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = true;
    saAttr.lpSecurityDescriptor = nullptr;

    CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0);
    SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0);
    SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0);

    //if (req.boddy.size() != 0) {
    //    exec += " | " + req.body;
    //}

    CreateChildProcess(env, exec);
    std::string tmp = "tmp";

    g_hInputFile = CreateFile((LPCWSTR)tmp.data(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr);
    WriteToPipe();
    resp = GetFromPipe();
#else

        while (i != _env.size()) {
            env += " " + _env[i];
            i++;
        }

        if (req.body.size() != 0) {
            file = popen(("echo " + req.body + " | " + "env -i" + env + " " +  _cgi).c_str(), "r");
        }
        else {
            file = popen(("env -i " + env + " " + _cgi).c_str(), "r");
        }

        if (file != NULL) {
            while (std::fgets(buf.data(), 128, file) != nullptr) {
                resp += buf.data();
            }
        }
        else {
            throw std::runtime_error("error CreateProces");
        }

#endif

    while ((pos = resp.find("\r\n")) != std::string::npos) {
        token = resp.substr(0, pos);
        while ((tokenPos = token.find(":")) != std::string::npos) {
            lilToken = token.substr(0, tokenPos);
            token.erase(0, tokenPos + 1);
            res.headers.insert({lilToken, token});
        }
        resp.erase(0, pos + 2);
    }
    res.body = resp;
    res.headers["Content-Length"] = std::to_string(res.body.size());
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new zia::modules::php::PhpCgi;
}
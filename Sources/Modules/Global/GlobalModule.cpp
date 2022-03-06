/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** GlobalModule
*/

#include "Modules/Global/GlobalModule.hpp"
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void zia::modules::global::GlobalCgi::Init(const ziapi::config::Node &cfg)
{
    try {
        for (const auto& e : cfg["modules"]["GLOBAL-CGI"]["script_filename"].AsArray()) {
            auto obj = e->AsDict();
            _paths.push_back({obj["url"]->AsString(), {obj["filePath"]->AsString(), obj["execPath"]->AsString()}});
        }
        _env.push_back("GATEWAY_INTERFACE=" + cfg["modules"]["GLOBAL-CGI"]["gateway_interface"].AsString());
        _env.push_back("PATH_INFO=" + cfg["modules"]["GLOBAL-CGI"]["path_info"].AsString());
        _env.push_back("PATH_TRANSLATED=" + cfg["modules"]["GLOBAL-CGI"]["path_translated"].AsString());
        _env.push_back("REMOTE_IDENT=" + cfg["modules"]["GLOBAL-CGI"]["remote_ident"].AsString());
        _env.push_back("REMOTE_USER=" + cfg["modules"]["GLOBAL-CGI"]["remote_user"].AsString());
        _env.push_back("SERVER_NAME=" + cfg["modules"]["GLOBAL-CGI"]["server_name"].AsString());
        _env.push_back("SERVER_PORT=" + cfg["modules"]["GLOBAL-CGI"]["server_port"].AsString());
        _env.push_back("SERVER_PROTOCOL=" + cfg["modules"]["GLOBAL-CGI"]["protocol"].AsString());
        _env.push_back("SERVER_SOFTWARE=" + cfg["modules"]["GLOBAL-CGI"]["server_software"].AsString());
        _env.push_back("REMOTE_HOST=" + cfg["modules"]["GLOBAL-CGI"]["remote_host"].AsString());
        _env.push_back("REMOTE_ADDR=" + cfg["modules"]["GLOBAL-CGI"]["ip_client"].AsString());
        _env.push_back("AUTH_TYPE=" + cfg["modules"]["GLOBAL-CGI"]["auth_type"].AsString());
        _env.push_back("REDIRECT_STATUS=200");
    }
    catch (const std::exception& e) {
        _initSetUp = false;
    }
}

ziapi::Version zia::modules::global::GlobalCgi::GetVersion() const noexcept 
{
    return {3, 1, 1};
}

ziapi::Version zia::modules::global::GlobalCgi::GetCompatibleApiVersion() const noexcept
{
    return {3, 1, 1};
}

const char *zia::modules::global::GlobalCgi::GetName() const noexcept
{
    return "Global cgi module";
}

const char *zia::modules::global::GlobalCgi::GetDescription() const noexcept
{
    return "Global cgi module for handle every cgi";
}

double zia::modules::global::GlobalCgi::GetHandlerPriority() const noexcept
{
    return 0.5;
}

bool zia::modules::global::GlobalCgi::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
{
    for (auto elem : _paths) {
        if (req.target.find(elem.first) == 0) {
            return _initSetUp;
        }
    }
    return false;
}

void zia::modules::global::GlobalCgi::EnvSetUp(const ziapi::http::Request &req) noexcept
{
        std::string query;

        for (auto elem : _paths) {
            if (req.target.find(elem.first) == 0) {
                _cgi = elem.second.second;
                _env.push_back("SCRIPT_FILENAME=" + elem.second.first);
                _env.push_back("SCRIPT_NAME=" + elem.second.first);
                break;
            }
        }
        try {
            _env.push_back("CONTENT_TYPE=" + req.headers.at("Content-Type"));
        }
        catch (const std::exception& e) {
            _env.push_back("CONTENT_TYPE=");
        }
        _env.push_back("REQUEST_METHOD=" + req.method);
        if (req.target.find("?") != req.target.npos) {
            query = req.target;
            query.erase(0, query.find("?") + 1);
        }
        _env.push_back("QUERY_STRING=" + query);
        _env.push_back("CONTENT_LENGTH=" + std::to_string(req.body.length()));
        _env.push_back("HTTP_ACCEPT=\"" + req.headers.at("Accept") + "\"");
        _env.push_back("HTTP_ACCEPT_LANGUAGE=fr");
        _env.push_back("HTTP_USER_AGENT=\"" + req.headers.at("User-Agent") + "\"");
        _env.push_back("HTTP_COOKIE=");
}

#if defined(_WIN32) || defined(_WIN64)
void zia::modules::global::GlobalCgi::WriteToPipe() noexcept
{
    DWORD dwRead;
    DWORD dwWritten;
    char chBuf[BUFSIZE];
    bool bSuccess = false;

    while (1) {
        bSuccess = ReadFile(_inputFile, chBuf, BUFSIZE, &dwRead, nullptr);
        if (!bSuccess || dwRead == 0) {
            break;
        }
        bSuccess = WriteFile(_childStd_IN_Wr, chBuf, dwRead, &dwWritten, nullptr);
        if (!bSuccess) {
            break;
        }
    }
    CloseHandle(_childStd_IN_Wr);
}

std::string zia::modules::global::GlobalCgi::GetFromPipe() noexcept
{
    int i = 0;
    DWORD dwRead;
    DWORD dwWritten;
    char chBuf[BUFSIZE];
    bool bSuccess = false;
    std::string tmp = {};

    while (1) {
        bSuccess = ReadFile(_childStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
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


void zia::modules::global::GlobalCgi::CreateChildProcess(std::string env, std::string exec)
{
    TCHAR szCmdline[] = TEXT("child");
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    bool bSuccess = false;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));

    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdOutput = _childStd_OUT_Wr;
    siStartInfo.hStdInput = _childStd_IN_Rd;
    siStartInfo.dwFlags = STARTF_USESTDHANDLES;

    bSuccess = CreateProcessA(nullptr, _strdup(exec.c_str()), nullptr, nullptr, true, 0, (LPVOID)env.data(), nullptr, &siStartInfo, &piProcInfo);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(_childStd_OUT_Wr);
    CloseHandle(_childStd_IN_Rd);
}
#endif


void zia::modules::global::GlobalCgi::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
{
    int i = 0;
    int pos = 0;
    int tokenPos = 0;
    FILE *file;
    std::array<char, 128> buf;
    std::string env = {};
    std::string resp = {};
    std::string token = {};
    std::string lilToken = {};

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

    CreatePipe(&_childStd_OUT_Rd, &_childStd_OUT_Wr, &saAttr, 0);
    SetHandleInformation(_childStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&_childStd_IN_Rd, &_childStd_IN_Wr, &saAttr, 0);
    SetHandleInformation(_childStd_IN_Wr, HANDLE_FLAG_INHERIT, 0);

    if (req.body.size() != 0) {
        exec = "echo " + req.body + " | " + exec;
    }

    CreateChildProcess(env, exec);
    std::string tmp = "tmp";

    _inputFile = CreateFileA(reinterpret_cast<LPCSTR>(tmp.data()), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr);
    WriteToPipe();
    resp = GetFromPipe();
#else

        while (i != _env.size()) {
            env += " " + _env[i];
            i++;
        }

        if (req.body.size() != 0) {
            file = popen(("echo \"" + req.body + "\" | " + "env -i" + env + " " + _cgi).c_str(), "r");
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
    if (resp == "Erreur") {
        ctx.insert({"ErrorOccured", std::pair(ziapi::http::Code::kInternalServerError, ziapi::http::reason::kInternalServerError)});
        return;
    }
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
    return new zia::modules::global::GlobalCgi;
}
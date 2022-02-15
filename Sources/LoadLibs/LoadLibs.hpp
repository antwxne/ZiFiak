//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once
#include <string>
#include  <filesystem>
#include <unordered_map>
#include "ziapi/Module.hpp"

class LoadLibs {
public:
    LoadLibs() = default;
    ~LoadLibs() = default;
    void openFilesAndStore(const std::string &file);
    void initLibs(ziapi::config::Node config);
    void getType();
    bool getTimeChange(std::string &file);

protected:
    std::vector<std::unique_ptr<ziapi::IModule>> _listLib;
    std::vector<std::unique_ptr<ziapi::IHandlerModule>> _handlerModules;
    std::vector<std::unique_ptr<ziapi::IPostProcessorModule>> _postProcessorModules;
    std::vector<std::unique_ptr<ziapi::IPreProcessorModule>> _preProcessorModules;
public:
    const std::vector<std::unique_ptr<ziapi::IModule>> &getListLib() const;

    const std::vector<std::unique_ptr<ziapi::IHandlerModule>> &getHandlerModules() const;

    const std::vector<std::unique_ptr<ziapi::IPostProcessorModule>> &getPostProcessorModules() const;

    const std::vector<std::unique_ptr<ziapi::IPreProcessorModule>> &getPreProcessorModules() const;

    const std::vector<std::unique_ptr<ziapi::INetworkModule>> &getNetWorkModules() const;

protected:
    std::vector<std::unique_ptr<ziapi::INetworkModule>> _netWorkModules;
    std::filesystem::file_time_type _lastTime;
};
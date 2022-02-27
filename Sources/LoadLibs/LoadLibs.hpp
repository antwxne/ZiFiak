//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once
#include <string>
#include  <filesystem>
#include <unordered_map>
#include "Watcher/Watcher.hpp"
#include "ziapi/Module.hpp"
#include "ziapi/Config.hpp"
#include "ziapi/Module.hpp"
#include "dylib/dylib.hpp"

class LoadLibs {
public:
    LoadLibs() = default;
    ~LoadLibs() = default;
    void openFilesAndStore(const std::string &file);
    void initLibs(ziapi::config::Node &config);
    void getType();
    void loadLibByFiles(const std::vector<Watcher::FileState> &files, ziapi::config::Node config);
    void loadSingleModule(const std::string &path);
    void deleteModule(const Watcher::FileState &e);
    void replaceModule(const Watcher::FileState &e);

protected:
    std::vector<std::pair<std::unique_ptr<ziapi::IModule>, std::string>> _listLib;

protected:
    std::vector<std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string>> _handlerModules;
    std::vector<std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string>>_postProcessorModules;
public:
    const std::vector<std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string>> &getHandlerModules() const;

    const std::vector<std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string>> &
    getPostProcessorModules() const;

    const std::vector<std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string>> &
    getPreProcessorModules() const;

    const std::vector<std::pair<std::unique_ptr<ziapi::INetworkModule>, std::string>> &getNetWorkModules() const;

protected:
    std::vector<std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string>> _preProcessorModules;
    std::vector<std::pair<std::unique_ptr<ziapi::INetworkModule>, std::string>> _netWorkModules;
    std::vector<dylib> _tmp;
};
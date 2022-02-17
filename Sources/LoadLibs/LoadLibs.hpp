//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once
#include <string>
#include  <filesystem>
#include <unordered_map>
#include "../Watcher/Watcher.hpp"
#include "ziapi/Module.hpp"
#include "../../build/ziapi-prefix/src/ziapi/include/ziapi/Config.hpp"
#include "../../build/ziapi-prefix/src/ziapi/include/ziapi/Module.hpp"

class LoadLibs {
public:
    LoadLibs() = default;
    ~LoadLibs() = default;
    void openFilesAndStore(const std::string &file);
    void initLibs(ziapi::config::Node config);
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
    std::vector<std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string>> _preProcessorModules;
    std::vector<std::pair<std::unique_ptr<ziapi::INetworkModule>, std::string>> _netWorkModules;

protected:
    std::filesystem::file_time_type _lastTime;
};
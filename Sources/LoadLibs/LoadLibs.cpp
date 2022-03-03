//
// Created by Clement Ruat on 02/02/2022.
//

#include  <filesystem>
#include <iostream>
#include <vector>
#include "LoadLibs/LoadLibs.hpp"
#include "dylib/dylib.hpp"
#include "ziapi/Module.hpp"

class INetworkModule;

void LoadLibs::deleteModule(const Watcher::FileState &e) {
    _handlerModules.erase(std::remove_if(_handlerModules.begin(), _handlerModules.end(),
                                         [&e](const std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string> &x) {
                                             return x.second == e.filepath;
                                         }
    ), _handlerModules.end());
    _postProcessorModules.erase(std::remove_if(_postProcessorModules.begin(), _postProcessorModules.end(),
                                               [&e](const std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string> &x) {
                                                   return x.second == e.filepath;
                                               }
    ), _postProcessorModules.end());
    _preProcessorModules.erase(std::remove_if(_preProcessorModules.begin(), _preProcessorModules.end(),
                                              [&e](const std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string> &x) {
                                                  return x.second == e.filepath;
                                              }
    ), _preProcessorModules.end());
    _netWorkModules.erase(std::remove_if(_netWorkModules.begin(), _netWorkModules.end(),
                                         [&e](const std::pair<std::unique_ptr<ziapi::INetworkModule>, std::string> &x) {
                                             return x.second == e.filepath;
                                         }
    ), _netWorkModules.end());
}

bool compareHandlerPriority(std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string> const &mod1,
std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string> const &mod2)
{
    return(mod1.first.get()->GetHandlerPriority() < mod2.first.get()->GetHandlerPriority());
}

bool comparePrePriority(std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string> const &mod1,
std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string> const &mod2)
{
    return(mod1.first.get()->GetPreProcessorPriority() < mod2.first.get()->GetPreProcessorPriority());
}

bool comparePostPriority(std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string> const & mod1,
std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string> const &mod2)
{
    return(mod1.first.get()->GetPostProcessorPriority() < mod2.first.get()->GetPostProcessorPriority());
}

void LoadLibs::sortModules(void)
{
    std::sort(_handlerModules.begin(), _handlerModules.end(), compareHandlerPriority);
    std::sort(_preProcessorModules.begin(), _preProcessorModules.end(), comparePrePriority);
    std::sort(_postProcessorModules.begin(), _postProcessorModules.end(), comparePostPriority);
}

void LoadLibs::loadSingleModule(const std::string &path) {
    _listLib.clear();
    try {
        dylib lib(path);
        auto libs = lib.get_function<ziapi::IModule *()>("LoadZiaModule");
        std::unique_ptr<ziapi::IModule> toto(libs());
        _tmp.push_back(std::move(lib));
        _listLib.push_back(std::make_pair(std::move(toto), path));

    } catch (dylib::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void LoadLibs::loadLibByFiles(const std::vector<Watcher::FileState> &files, ziapi::config::Node config) {
    for (auto &e: files) {
        if (e.state == Watcher::State::DEL) {
            deleteModule(e);
        } else if (e.state == Watcher::State::ADD) {
            loadSingleModule(e.filepath);
            initLibs(config);
            getType();
        } else if (e.state == Watcher::State::MOD) {
            deleteModule(e);
            loadSingleModule(e.filepath);
            initLibs(config);
            getType();
        }
    }
    sortModules();
}

void LoadLibs::openFilesAndStore(const std::string &file) {
    std::string tmp;
    for (auto &x: std::filesystem::directory_iterator(file)) {
        try {
            tmp = x.path().string();
            if (tmp == file)
                continue;
            dylib lib(x.path().string());
            auto libs = lib.get_function<ziapi::IModule *()>("LoadZiaModule");
            std::unique_ptr<ziapi::IModule> toto(libs());
            _tmp.push_back(std::move(lib));
            _listLib.push_back(std::make_pair(std::move(toto), tmp));
        } catch (const dylib::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    };
}

void LoadLibs::initLibs(ziapi::config::Node &config) {
    for (auto &e: _listLib) {
        e.first->Init(config);
    }
}

void LoadLibs::getType() {
    for (auto &e: _listLib) {
        auto handlerType = dynamic_cast<ziapi::IHandlerModule *>(e.first.get());
        auto postProcessorType = dynamic_cast<ziapi::IPostProcessorModule *>(e.first.get());
        auto preProcessorType = dynamic_cast<ziapi::IPreProcessorModule *>(e.first.get());
        auto netWorkType = dynamic_cast<ziapi::INetworkModule *>(e.first.get());

        e.first.release();

        if (handlerType) {
            _handlerModules.emplace_back(std::make_pair(handlerType, e.second));
        }
        if (postProcessorType) {
            _postProcessorModules.emplace_back(std::make_pair(postProcessorType, e.second));
        }
        if (preProcessorType) {
            _preProcessorModules.emplace_back(std::make_pair(preProcessorType, e.second));
        }
        if (netWorkType) {
            _netWorkModules.emplace_back(std::make_pair(netWorkType, e.second));
        }
    }
}

const std::vector<std::pair<std::unique_ptr<ziapi::IHandlerModule>, std::string>> &LoadLibs::getHandlerModules() const {
    return _handlerModules;
}

const std::vector<std::pair<std::unique_ptr<ziapi::IPostProcessorModule>, std::string>> &
LoadLibs::getPostProcessorModules() const {
    return _postProcessorModules;
}

const std::vector<std::pair<std::unique_ptr<ziapi::IPreProcessorModule>, std::string>> &
LoadLibs::getPreProcessorModules() const {
    return _preProcessorModules;
}

const std::vector<std::pair<std::unique_ptr<ziapi::INetworkModule>, std::string>> &LoadLibs::getNetWorkModules() const {
    return _netWorkModules;
}

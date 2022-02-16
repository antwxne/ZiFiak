//
// Created by Clement Ruat on 02/02/2022.
//

#include "LoadLibs/LoadLibs.hpp"
#include  <filesystem>
#include <iostream>
#include <vector>
#include "dylib/dylib.hpp"
#include "ziapi/Module.hpp"
#include "../Server/Server.hpp"

bool LoadLibs::getTimeChange(std::string &file) {
    auto tmp= std::filesystem::last_write_time(file);
    if (_lastTime != tmp) {
        _lastTime = tmp;
        return true;
    }
    return false;
}

void LoadLibs::loadLibByFiles(const std::vector<std::string> &files) {
    for (auto &e : files) {

    }
}

void LoadLibs::openFilesAndStore(const std::string &file) {
    std::string tmp;
    for (auto &x: std::filesystem::directory_iterator()) {
        try {
            tmp = x.path().string();
            if (tmp == file)
                continue;
            dylib lib(x.path().string());
            auto libs = lib.get_function<ziapi::IModule * ()>("LoadZiaModule");
            std::unique_ptr<ziapi::IModule> toto(libs());
            _listLib.push_back(std::move(toto));
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    };
}

void LoadLibs::initLibs(ziapi::config::Node config) {
    for (auto &e: _listLib) {
        e->Init(config);
    }
}

void LoadLibs::getType() {
    for (auto &e: _listLib) {
        auto handlerType = dynamic_cast<ziapi::IHandlerModule *>(e.get());
        auto postProcessorType = dynamic_cast<ziapi::IPostProcessorModule *>(e.get());
        auto preProcessorType = dynamic_cast<ziapi::IPreProcessorModule *>(e.get());
        auto netWorkType = dynamic_cast<ziapi::INetworkModule *>(e.get());

        if (handlerType) {
            _handlerModules.emplace_back(handlerType);
        }
        if (postProcessorType) {
            _postProcessorModules.emplace_back(postProcessorType);
        }
        if (preProcessorType) {
            _preProcessorModules.emplace_back(preProcessorType);
        }
        if (netWorkType) {
            _netWorkModules.emplace_back(netWorkType);
        }
    }
}

const std::vector<std::unique_ptr<ziapi::IModule>> &LoadLibs::getListLib() const {
    return _listLib;
}

const std::vector<std::unique_ptr<ziapi::IHandlerModule>> &LoadLibs::getHandlerModules() const {
    return _handlerModules;
}

const std::vector<std::unique_ptr<ziapi::IPostProcessorModule>> &LoadLibs::getPostProcessorModules() const {
    return _postProcessorModules;
}

const std::vector<std::unique_ptr<ziapi::IPreProcessorModule>> &LoadLibs::getPreProcessorModules() const {
    return _preProcessorModules;
}

const std::vector<std::unique_ptr<ziapi::INetworkModule>> &LoadLibs::getNetWorkModules() const {
    return _netWorkModules;
}

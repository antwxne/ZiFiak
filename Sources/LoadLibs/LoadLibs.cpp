//
// Created by Clement Ruat on 02/02/2022.
//

#include "LoadLibs/LoadLibs.hpp"
#include  <filesystem>
#include <iostream>
#include "dylib/dylib.hpp"
#include "ziapi/Module.hpp"

void LoadLibs::openFilesAndStore(std::string &file) {
    std::string tmp;
    for (auto &x: std::filesystem::directory_iterator()) {
        try {
            tmp = x.path();
            if (tmp == file)
                continue;
            dylib lib(x.path());
            auto libs = lib.get_function<ziapi::IModule * ()>("LoadZiaModule");
            listLib.push_back(libs);
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    };
}

void LoadLibs::initLibs() {
    for (auto &e: listLib) {
        e.init();
    }
}

void LoadLibs::getType() {
    for (auto &e: listLib) {
        auto handlerType = dynamic_cast<ziapi::IHandlerModule>(e);
        auto postProcessorType = dynamic_cast<ziapi::IPostProcessorModule>(e);
        auto preProcessorType = dynamic_cast<ziapi::IPreProcessorModule>(e);
        auto netWorkType = dynamic_cast<ziapi::INetworkModule>(e);

        if (handlerType) {
            handlerModules.push_back(e);
        }
        if (postProcessorType) {
            postProcessorModules.push_back(e);
        }
        if (preProcessorType) {
            preProcessorModules.push_back(e);
        }
        if (netWorkType) {
            netWorkModules.push_back(e);
        }
    }
}
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
            tmp = std::string(x.path().c_str());
            if (tmp == file)
                continue;
            dylib lib(x.path());
            auto libs = lib.get_function<ziapi::IModule * ()>("LoadZiaModule");
            std::unique_ptr<ziapi::IModule> toto(libs());
            listLib.push_back(std::move(toto));
        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    };
}

void LoadLibs::initLibs() {
    for (auto &e: listLib) {
 //    e->Init();
    }
}

void LoadLibs::getType() {
    for (auto &e: listLib) {
        auto handlerType = dynamic_cast<ziapi::IHandlerModule *>(e.get());
        auto postProcessorType = dynamic_cast<ziapi::IPostProcessorModule *>(e.get());
        auto preProcessorType = dynamic_cast<ziapi::IPreProcessorModule *>(e.get());
        auto netWorkType = dynamic_cast<ziapi::INetworkModule *>(e.get());

        if (handlerType) {
            handlerModules.emplace_back(handlerType);
        }
        if (postProcessorType) {
            postProcessorModules.emplace_back(postProcessorType);
        }
        if (preProcessorType) {
            preProcessorModules.emplace_back(preProcessorType);
        }
        if (netWorkType) {
            netWorkModules.emplace_back(netWorkType);
        }
    }
}
//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once
#include <string>
#include "ziapi/Module.hpp"

class LoadLibs {
public:
    LoadLibs() = default;
    ~LoadLibs() = default;
    void openFilesAndStore(std::string &file);
    void initLibs(ziapi::config::Node config);
    void getType();

protected:
    std::vector<std::unique_ptr<ziapi::IModule>> listLib;
    std::vector<std::unique_ptr<ziapi::IHandlerModule>> handlerModules;
    std::vector<std::unique_ptr<ziapi::IPostProcessorModule>> postProcessorModules;
    std::vector<std::unique_ptr<ziapi::IPreProcessorModule>> preProcessorModules;
    std::vector<std::unique_ptr<ziapi::INetworkModule>> netWorkModules;


};
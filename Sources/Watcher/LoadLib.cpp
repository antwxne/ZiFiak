//
// Created by Clement Ruat on 02/02/2022.
//

#include "LoadLib.hpp"

void LoadLib::dlOPenLib(const std::string &lib) {
    void *tmp = dlopen(lib.c_str(), RTLD_NOW);
    if (!tmp)
        throw std::runtime_error(dlerror());
    _modules.push_back(std::make_shared<void *>(tmp));
}

LoadLib::~LoadLib() {
   // dlclose();
}
//
// Created by Clement Ruat on 02/02/2022.
//

#include "LoadLib.hpp"

#if defined(_WIN64)
void LoadLib::dlOPenLib(const std::string lib) {
    auto tmp = LoadLibraryA(lib.c_str());
    if (!tmp) {
        throw std::runtime_error("dlerror()");// dlerror() avant
    _modules.push_back(tmp);
    }
}
LoadLib::~LoadLib() {
    //jsp
}
#else
void LoadLib::dlOPenLib(const std::string &lib) {
    void *tmp = dlopen(lib.c_str(), RTLD_NOW);
    if (!tmp)
        throw std::runtime_error(dlerror());
    _modules.push_back(tmp);
}

LoadLib::~LoadLib() {
    // dlclose();
}
#endif
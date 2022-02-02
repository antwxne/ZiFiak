//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once

#include <string>
#include <iostream>
#include <dlfcn.h>
#include <vector>

class LoadLib {
public:
    LoadLib() = default;

    ~LoadLib();

    template<typename T>
    std::function<T> loadingLib(const std::string &symbol) const {
        T *tmp = reinterpret_cast<T *>(dlsym(*(_modules[(_modules.size() -1)]), symbol.c_str()));
        if (!tmp)
            throw std::runtime_error(dlerror());
        return std::function<T>(tmp);
    }

    void dlOPenLib(const std::string &lib);

private:
    std::vector<std::shared_ptr<void *>> _modules;
};

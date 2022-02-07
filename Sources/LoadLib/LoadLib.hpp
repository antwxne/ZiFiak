//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once

#include <string>
#include <iostream>
#if defined(_WIN64)
    #include <Windows.h>
    #define  Loading extern "C" __declspec(dllexport)
#else
    #include <dlfcn.h>
    #define Loading extern "C"
#endif
#include <vector>
#include <memory>
#include <functional>

class LoadLib {
public:
    LoadLib() = default;

    ~LoadLib();
#if defined(_WIN64)
    template<typename T>
    std::function<T> loadingLib(const std::string &symbol) const {
        T *tmp = reinterpret_cast<T *>(GetProcAddress((_modules[(_modules.size() -1)]), symbol.c_str()));
        if (!tmp)
            throw std::runtime_error(dlerror());
        return std::function<T>(tmp);
    }
#else
    template<typename T>
    std::function<T> loadingLib(const std::string &symbol) const {
        T *tmp = reinterpret_cast<T *>(dlsym((_modules[(_modules.size() -1)]), symbol.c_str()));
        if (!tmp)
            throw std::runtime_error(dlerror());
        return std::function<T>(tmp);
    }
#endif

void dlOPenLib(const std::string &lib);

private:
#if defined(_WIN64)
    std::vector<HINSTANCE> _modules;
#else
    std::vector<void *> _modules;
#endif
};

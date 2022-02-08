//
// Created by Clement Ruat on 02/02/2022.
//

#pragma  once
#include <string>

class LoadLibs {
public:
    LoadLibs() = default;
    ~LoadLibs() = default;
    void openFilesAndStore(std::string &file);
};
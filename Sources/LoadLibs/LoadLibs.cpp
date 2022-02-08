//
// Created by Clement Ruat on 02/02/2022.
//

#include "LoadLibs/LoadLibs.hpp"
#include  <filesystem>
#include <iostream>
#include "dylib/dylib.hpp"

void LoadLibs::openFilesAndStore(std::string &file) {
    std::string tmp;
    for (auto &x: std::filesystem::directory_iterator()) {
        try {
            tmp = x.path();

        } catch (const std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    };
}
/*
** EPITECH PROJECT, 2022
** B-YEP-500-PAR-5-1-zia-arthur.fouquet
** File description:
** Watcher
*/

#pragma once
#include <string>
#include <map>
#include <chrono>
#include <vector>
#include <filesystem>

namespace Watcher {

static const std::string ModulesPath = "Modules/";

class Watcher {
    public:
        Watcher(const std::string &path);
        ~Watcher();
        void update();

    private:
        std::vector<std::string> getFilesInFolder(const std::string &path);
        std::string _basicPath;
        std::map<std::filesystem::path, std::chrono::nanoseconds> _saves;
};

}
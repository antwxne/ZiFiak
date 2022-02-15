/*
** EPITECH PROJECT, 2022
** B-YEP-500-PAR-5-1-zia-arthur.fouquet
** File description:
** Watcher
*/

#include "Watcher.hpp"
#include <iostream>

namespace Watcher {

Watcher::Watcher(const std::string &path) : _basicPath(path)
{
}

Watcher::~Watcher()
{
}

std::vector<std::string> Watcher::getFilesInFolder(const std::string &path)
{
    std::vector<std::string> files;

    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        auto path = entry.path();

        if (std::filesystem::is_directory(entry)) {
            auto tmpVector = getFilesInFolder(path);
            files.insert( files.end(), tmpVector.begin(), tmpVector.end() );
            continue;
        } else if (!std::filesystem::exists(path)) {
            continue;
        }
        auto modifiedTime = std::filesystem::last_write_time(entry).time_since_epoch();
        if (_saves.find(path) == _saves.end()) {
            _saves[path] = modifiedTime;
            files.emplace_back(path);
        } else if (_saves.at(path) != modifiedTime) {
            files.emplace_back(path);
            _saves[path] = modifiedTime;
        }
    }
    return files;
}

void Watcher::CreateNewSave(const std::string &path, std::chrono::microseconds &modifiedTime)
{
    _saves[path] = modifiedTime;
}

void Watcher::update()
{
    auto modifiedFiles = getFilesInFolder(_basicPath);

    if (modifiedFiles.empty())
        return;
    std::cout << "modified files:" << std::endl;
    for (auto &it : modifiedFiles) {
        std::cout << it << std::endl;
    }
}

}
/*
** EPITECH PROJECT, 2022
** B-YEP-500-PAR-5-1-zia-arthur.fouquet
** File description:
** Watcher
*/

#include "Watcher.hpp"
#include <iostream>
#include <algorithm>

namespace Watcher {

Watcher::Watcher(const std::string &path) : _basicPath(path)
{
}

Watcher::~Watcher()
{
}

std::vector<FileState> Watcher::getFilesInFolder(const std::string &folderPath)
{
    std::vector<FileState> files;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        auto path = entry.path();

        if (std::filesystem::is_directory(entry)) {
            auto tmpVector = getFilesInFolder(path);
            files.insert( files.end(), tmpVector.begin(), tmpVector.end());
            continue;
        } else if (!std::filesystem::exists(path)) {
            continue;
        }
        auto modifiedTime = std::filesystem::last_write_time(entry).time_since_epoch();
        if (_saves.find(path) == _saves.end()) {
            _saves[path] = modifiedTime;
            files.emplace_back(FileState{path, State::ADD});
        } else if (_saves.at(path) != modifiedTime) {
            files.emplace_back(FileState{path, State::MOD});
            _saves[path] = modifiedTime;
        }
    }
    return (files);
}

std::vector<FileState> Watcher::checkDeletedFiles()
{
    std::vector<FileState> files;
    auto it = _saves.cbegin();

    while (it != _saves.cend()) {
        if (!std::filesystem::exists(it->first)) {
            files.emplace_back(FileState{it->first, State::DEL});
            it = _saves.erase(it);
        } else {
            ++it;
        }
    }
    return (files);
}


std::vector<FileState> Watcher::update()
{
    auto modifiedFiles = getFilesInFolder(_basicPath);
    auto deletedFiles = checkDeletedFiles();

    modifiedFiles.insert(modifiedFiles.end(), deletedFiles.begin(), deletedFiles.end());
    return (modifiedFiles);
}

}
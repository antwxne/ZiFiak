/*
** EPITECH PROJECT, 2022
** B-YEP-500-PAR-5-1-zia-arthur.fouquet
** File description:
** Watcher
*/

#include "Watcher.hpp"
#include <iostream>

namespace Watcher {

Watcher::Watcher(bool &changes) : _changes(changes)
{
}

Watcher::~Watcher()
{
    _thread.join();
}

void Watcher::init(const std::string &path)
{
    auto systemPath = std::filesystem::status(path);

    _basicPath = path;
    if (!std::filesystem::is_directory(systemPath)) {
        searchFile();
        _thread = std::thread(&Watcher::updateFile, this);
    } else {
        searchFiles();
        _thread = std::thread(&Watcher::update, this);
    }
}

std::vector<FileState> Watcher::getFilesInFolder(const std::string &folderPath)
{
    std::vector<FileState> files = {};

    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        auto path = entry.path();

        if (std::filesystem::is_directory(path)) {
            auto tmpVector = getFilesInFolder(path.string());
            files.insert(files.end(), tmpVector.begin(), tmpVector.end());
            continue;
        } else if (!std::filesystem::exists(path)) {
            continue;
        }
        auto modifiedTime = std::filesystem::last_write_time(entry).time_since_epoch();
        if (_saves.find(path) == _saves.end()) {
            _saves[path] = modifiedTime;
            files.push_back(FileState{path.string(), State::ADD});
        } else if (_saves.at(path) != modifiedTime) {
            files.push_back(FileState{path.string(), State::MOD});
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
            files.push_back(FileState{it->first.string(), State::DEL});
            it = _saves.erase(it);
        } else {
            ++it;
        }
    }
    return (files);
}

void Watcher::setUpdatedFiles(std::vector<FileState> &files)
{
    std::scoped_lock lock(_mutex);
    _changes = true;
    _modifiedFiles.insert(_modifiedFiles.end(), files.begin(), files.end());
}

std::vector<FileState> Watcher::getChanges()
{
    std::scoped_lock lock(_mutex);
    std::vector<FileState> tmp(_modifiedFiles);

    _modifiedFiles.clear();
    return (tmp);
}

void Watcher::searchFiles()
{
    auto modifiedFiles = getFilesInFolder(_basicPath);
    auto deletedFiles = checkDeletedFiles();

    modifiedFiles.insert(modifiedFiles.end(), deletedFiles.begin(), deletedFiles.end());
    if (!modifiedFiles.empty()) {
        setUpdatedFiles(modifiedFiles);
    }
}

void Watcher::searchFile()
{
    std::vector<FileState> files;

    if (!std::filesystem::exists(std::filesystem::status(_basicPath))) {
        if (_saves.size()) {
            files.push_back(FileState{_basicPath, State::DEL});
            _saves.clear();
            setUpdatedFiles(files);
        }
        return;
    }
    auto modifiedTime = std::filesystem::last_write_time(_basicPath).time_since_epoch();
    if (_saves.find(_basicPath) == _saves.end()) {
        _saves[_basicPath] = modifiedTime;
        files.push_back(FileState{_basicPath, State::ADD});
    } else if (_saves.at(_basicPath) != modifiedTime) {
        files.push_back(FileState{_basicPath, State::MOD});
        _saves[_basicPath] = modifiedTime;
    }
    if (!files.empty()) {
        setUpdatedFiles(files);
    }
}

void Watcher::update()
{
    while (1) {
        searchFiles();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Watcher::updateFile()
{
    while (1) {
        searchFile();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

}
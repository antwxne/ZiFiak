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
#include <mutex>
#include <thread>

namespace Watcher {

enum State {
    ADD,
    MOD,
    DEL
};

struct FileState {
    std::string filepath;
    State state;
};

static const std::string ModulesPath = "Modules/";

class Watcher {
    public:
        Watcher(const std::string &path, bool &changes);
        ~Watcher();
        void update();
        std::vector<FileState> getChanges();

    private:
        void setUpdatedFiles(std::vector<FileState> &files);
        std::vector<FileState> getFilesInFolder(const std::string &path);
        std::vector<FileState> checkDeletedFiles();
        std::string _basicPath;
        std::map<std::filesystem::path, std::chrono::nanoseconds> _saves;
        std::vector<FileState> _modifiedFiles;
        bool &_changes;
        std::thread _thread;
        std::mutex _mutex;
};

}
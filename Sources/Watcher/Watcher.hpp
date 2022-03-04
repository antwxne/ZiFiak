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

class Watcher {
    public:
        Watcher(bool &changes);
        ~Watcher();
        void init(const std::string &path);
        void update();
        void updateFile();
        std::vector<FileState> getChanges();

    private:
        void setUpdatedFiles(std::vector<FileState> &files);
        void searchFiles();
        void searchFile();
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
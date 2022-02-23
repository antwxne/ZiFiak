/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include <exception>
#include "Debug/Debug.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "Queue/RequestQueue.hpp"
#include "Queue/ResponseQueue.hpp"
#include <iostream>

zia::server::Server::Server() : _isModuleChange(false), _moduleWatcher(Watcher::ModulesPath, _isModuleChange), _isRunning(false) {
}

void zia::server::Server::init(const std::string &filepath) {
    Debug::log("init server");

    ConfigParser confParser;

    try {
        this->_serverConfig = ConfigParser::loadFromFile(filepath);
        auto pathDirectory = zia::server::Server::getPathDirectory();
        _loadLibs.loadLibByFiles(_moduleWatcher.getChanges(), _serverConfig);
        Debug::log("config load successfully loaded");
    } catch (const std::runtime_error &e) {
        Debug::warn("failed to load config file: " + std::string(e.what()));
        this->_serverConfig = Node(ziapi::config::Undefined{});
    }
}

const std::string zia::server::Server::getPathDirectory() const {
    try {
        return _serverConfig["directory"].AsString();
    } catch (std::runtime_error &e) {
        return "./modules";
    }
}

void zia::server::Server::threadPool(zia::container::RequestQueue &request, zia::container::ResponseQueue &responses)
{
    while (_isRunning) {
        auto curr = request.Pop();
        if (curr == std::nullopt) {
            continue;
        }
        // pipeline d'olivier
    }
}

void zia::server::Server::run() {
    zia::container::RequestQueue requests;
    zia::container::ResponseQueue responses;

    _isRunning = true;
    for (auto &module : _allNetWorkModules) {
        module.first->Run(requests, responses);
    }
    _threadPool.emplace_back(std::thread(&threadPool, this, requests, responses));
    while (1) {

        if (_isModuleChange) {
            _loadLibs.loadLibByFiles(_moduleWatcher.getChanges(), _serverConfig);
            _isModuleChange = false;
        }
    }
    _isRunning = false;
    Debug::log("server running");
}

const ziapi::config::Node &zia::server::Server::getServerConfig() const {
    return _serverConfig;
}
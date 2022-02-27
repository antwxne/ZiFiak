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

void zia::server::Server::handleModule(const std::unique_ptr<ziapi::IHandlerModule> &process, std::pair<ziapi::http::Request, ziapi::http::Context> &req, zia::container::ResponseQueue &handlerResponses)
{
    std::scoped_lock lock(_mutex);
    ziapi::http::Response response;

    process->Handle(req.second, req.first, response);
    handlerResponses.push_back(std::make_pair(response, req.second));
}

void zia::server::Server::pipeLine(std::pair<ziapi::http::Request, ziapi::http::Context> &req, zia::container::ResponseQueue &responses)
{
    for (auto &module : _loadLibs.getPreProcessorModules()) {
        if (module.first->ShouldPreProcess(req.second, req.first)) {
            module.first->PreProcess(req.second, req.first);
        }
    }
    for (auto &module : _loadLibs.getHandlerModules()) {
        if (module.first->ShouldHandle(req.second, req.first)) {
            handleModule(module.first, req, responses);
        }
    }
    for (auto &module : _loadLibs.getPostProcessorModules()) {
        for (auto &response : responses) {
            if (module.first->ShouldPostProcess(req.second, response.first)) {
                module.first->PostProcess(response.second, response.first);
            }
        }
    }
}

void zia::server::Server::threadPool(zia::container::RequestQueue &request, zia::container::ResponseQueue &responses)
{
    while (_isRunning) {
        if (request.size() != 0)
            std::cout << request.size() << std::endl;
        auto curr = request.Pop();
        if (curr == std::nullopt) {
            continue;
        }
        std::thread(&zia::server::Server::pipeLine, this, std::ref(curr.value()), std::ref(responses));
    }
}

void zia::server::Server::run() {
    zia::container::RequestQueue requests;
    zia::container::ResponseQueue responses;

    _isRunning = true;
    for (auto &module : _loadLibs.getNetWorkModules()) {
        module.first->Run(requests, responses);
    }
    _threadPool.emplace_back(std::thread(&zia::server::Server::threadPool, this, std::ref(requests), std::ref(responses)));
    std::cout << _loadLibs.getHandlerModules().size() << std::endl;
    std::cout << _loadLibs.getPreProcessorModules().size() << std::endl;
    std::cout << _loadLibs.getPostProcessorModules().size() << std::endl;
    std::cout << _loadLibs.getNetWorkModules().size() << std::endl;
    while (1) {
        if (_isModuleChange) {
            _loadLibs.loadLibByFiles(_moduleWatcher.getChanges(), _serverConfig);
            _isModuleChange = false;
            std::cout << _loadLibs.getHandlerModules().size() << std::endl;
            std::cout << _loadLibs.getPreProcessorModules().size() << std::endl;
            std::cout << _loadLibs.getPostProcessorModules().size() << std::endl;
            std::cout << _loadLibs.getNetWorkModules().size() << std::endl;
        }
    }
    _isRunning = false;
    Debug::log("server running");
}

const ziapi::config::Node &zia::server::Server::getServerConfig() const {
    return _serverConfig;
}
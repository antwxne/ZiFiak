/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include <exception>
#include <iostream>
#include "Debug/Debug.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

zia::server::Server::Server() : _isModuleChange(false), _isConfigChange(false),
                                _moduleWatcher(_isModuleChange), _configWatcher(_isConfigChange), _isRunning(false) {
}

zia::server::Server::~Server()
{
    for (auto &it : _threadPool) {
        it.join();
    }
}

void zia::server::Server::init(const std::string &filepath) {
    Debug::log("init server");

    ConfigParser confParser;

    try {
        _configWatcher.init(filepath);
        _configWatcher.getChanges();
        this->_serverConfig = ConfigParser::loadFromFile(filepath);
        auto pathDirectory = zia::server::Server::getPathDirectory();
        _moduleWatcher.init(pathDirectory);
        _loadLibs.loadLibByFiles(_moduleWatcher.getChanges(), _serverConfig);
        Debug::log("config load successfully loaded");
    } catch (const std::runtime_error &e) {
        std::cout << "CRASH " << e.what() << std::endl;
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
    ziapi::http::Response response;
    response.Bootstrap();
    std::string plop("<!DOCTYPE html>\n"
                     "<html>\n"
                     "    <head>\n"
                     "        <title>Example</title>\n"
                     "    </head>\n"
                     "    <body>\n"
                     "        <p>This is an example of a simple HTML page with one paragraph.</p>\n"
                     "    </body>\n"
                     "</html>");
    response.body = plop;
    response.headers[ziapi::http::header::kContentLength] = std::to_string(plop.size());
    for (auto &module : _loadLibs.getPreProcessorModules()) {
        if (module.first->ShouldPreProcess(req.second, req.first)) {
            module.first->PreProcess(req.second, req.first);
        }
    }
    for (auto &module : _loadLibs.getHandlerModules()) {
        if (module.first->ShouldHandle(req.second, req.first)) {
            module.first->Handle(req.second, req.first, response);
            // handleModule(module.first, req, response);
        }
    }
    for (auto &module : _loadLibs.getPostProcessorModules()) {
        // for (auto &response : responses) {
            if (module.first->ShouldPostProcess(req.second, req.first, response)) {
                module.first->PostProcess(req.second, req.first, response);
            }
        // }
    }
    // std::cout << "XXX: " << std::any_cast<std::string>(req.second["client.socket.address"]) << " " << std::any_cast<std::uint16_t>(req.second["client.socket.port"])  << std::endl;
    // for (auto &elem : req.second) {
    //     std::cout << "MAP: " << elem.first << std::endl;
    // }
    responses.emplace_back(std::make_pair(response, req.second));
}

void zia::server::Server::threadPool(zia::container::RequestQueue &request, zia::container::ResponseQueue &responses)
{
    while (_isRunning) {
        auto curr = request.Pop();
        if (curr == std::nullopt) {
            continue;
        }
        pipeLine(curr.value(), responses);
        // std::thread(&zia::server::Server::pipeLine, this, std::ref(curr.value()), std::ref(responses));
    }
}

void zia::server::Server::threadPoolNetwork(const std::unique_ptr<ziapi::INetworkModule> &network)
{
    zia::container::RequestQueue requests;
    zia::container::ResponseQueue responses;

    network->Run(requests, responses);
    while (_isRunning) {
        threadPool(requests, responses);
    }
}

void zia::server::Server::run() {
    _isRunning = true;
    for (auto &module : _loadLibs.getNetWorkModules()) {
        // module.first->Run(requests, responses);
        _threadPool.emplace_back(std::thread(&zia::server::Server::threadPoolNetwork, this, std::ref(module.first)));
    }
    while (1) {
        if (_isConfigChange) {
            if (_configWatcher.getChanges()[0].state == Watcher::DEL)
                continue;
            std::cout << "changeConfig" << std::endl;
            std::cout << "path directory " << zia::server::Server::getPathDirectory() << std::endl;
            _loadLibs.openFilesAndStore(zia::server::Server::getPathDirectory());
            std::cout << "openFilesAndStore" << std::endl;
            _loadLibs.initLibs(_serverConfig);
            std::cout << "initLibs" << std::endl;
            _loadLibs.getType();
            std::cout << "getType" << std::endl;
            _loadLibs.sortModules();
            std::cout << "sortModules" << std::endl;
           // _loadLibs.sortModules();
            std::cout << "sort" << std::endl;
            _isConfigChange = false;
            std::cout << "toto" << std::endl;
            std::cout << _loadLibs.getPostProcessorModules().size() << std::endl;
            std::cout << _loadLibs.getPreProcessorModules().size() << std::endl;
            std::cout << _loadLibs.getNetWorkModules().size() << std::endl;
            std::cout << _loadLibs.getHandlerModules().size() << std::endl;

        }
        if (_isModuleChange) {
            std::cout << "zouzou" << std::endl;
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
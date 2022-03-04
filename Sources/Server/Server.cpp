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
        }
    }
    for (auto &module : _loadLibs.getPostProcessorModules()) {
        if (module.first->ShouldPostProcess(req.second, req.first, response)) {
            module.first->PostProcess(req.second, req.first, response);
        }
    }
    // std::cout << "XXX: " << std::any_cast<std::string>(req.second["client.socket.address"]) << " " << std::any_cast<std::uint16_t>(req.second["client.socket.port"])  << std::endl;
    // for (auto &elem : req.second) {
    //     std::cout << "MAP: " << elem.first << std::endl;
    // }
    responses.emplace_back(std::make_pair(response, req.second));
}

void zia::server::Server::threadPool(zia::container::RequestQueue &request, zia::container::ResponseQueue &responses)
{
    while (getIsRunning()) {
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
    while (getIsRunning()) {
        threadPool(requests, responses);
    }
    network->Terminate();
}

void zia::server::Server::terminateNetwork()
{
    setIsRunning(false);
    for (auto &it: _threadPool) {
        it.join();
    }
    _threadPool.clear();
}

void zia::server::Server::initNetwork()
{
    setIsRunning(true);
    for (auto &module : _loadLibs.getNetWorkModules()) {
        _threadPool.emplace_back(std::thread(&zia::server::Server::threadPoolNetwork, this, std::ref(module.first)));
    }
}

void zia::server::Server::run() {
    initNetwork();
    while (1) {
        if (_isConfigChange) {
            auto newConfig = _configWatcher.getChanges();
            if (newConfig[0].state == Watcher::DEL)
                continue;
            terminateNetwork();
            try {
                _serverConfig = ConfigParser::loadFromFile(newConfig[0].filepath);
            } catch (const std::exception &e) {
                Debug::warn("failed to load config file: " + std::string(e.what()));
                this->_serverConfig = Node(ziapi::config::Undefined{});
            }
            _loadLibs.openFilesAndStore(zia::server::Server::getPathDirectory());
            _loadLibs.initLibs(_serverConfig);
            _loadLibs.getType();
            _loadLibs.sortModules();
            initNetwork();
            _isConfigChange = false;
            // std::cout << _loadLibs.getHandlerModules().size() << std::endl;
            // std::cout << _loadLibs.getNetWorkModules().size() << std::endl;
            // std::cout << _loadLibs.getPostProcessorModules().size() << std::endl;
            // std::cout << _loadLibs.getPreProcessorModules().size() << std::endl;
        }
        if (_isModuleChange) {
            terminateNetwork();
            _loadLibs.loadLibByFiles(_moduleWatcher.getChanges(), _serverConfig);
            initNetwork();
            _isModuleChange = false;
        }
    }
    Debug::log("server running");
}

const ziapi::config::Node &zia::server::Server::getServerConfig() const {
    return _serverConfig;
}
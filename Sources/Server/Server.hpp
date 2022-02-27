/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_SERVER_HPP
#define BASIC_SERVER_SERVER_HPP

#include "ziapi/Config.hpp"
#include "LoadLibs/LoadLibs.hpp"
#include "Watcher/Watcher.hpp"
#include "Queue/RequestQueue.hpp"
#include "Queue/ResponseQueue.hpp"

namespace zia::server {
class Server {
    using Node = ziapi::config::Node;
public:
    Server();
    ~Server() = default;
    Server(const Server &) = delete;
    Server(const Server &&) = delete;
    Server &operator=(const Server &) = delete;
    void init(const std::string &filepath);
    void run();
    const std::string getPathDirectory() const;

private:
    // template<typename T, typename Y, typename U>
    // void handleModule(std::function<void(T, Y)> &module, U &req)
    // {
    //     std::scoped_lock lock(_mutex);

    //     module(req.second, req.first);
    // }
    void handleModule(const std::unique_ptr<ziapi::IHandlerModule> &process, std::pair<ziapi::http::Request, ziapi::http::Context> &req, zia::container::ResponseQueue &handlerResponses);
    void threadPool(zia::container::RequestQueue &request, zia::container::ResponseQueue &responses);
    void pipeLine(std::pair<ziapi::http::Request, ziapi::http::Context> &req, zia::container::ResponseQueue &responses);
    Node _serverConfig;
    LoadLibs _loadLibs;
    bool _isModuleChange;
    Watcher::Watcher _moduleWatcher;
    std::vector<std::thread> _threadPool;
    std::mutex _mutex;
    bool _isRunning;

public:
    const ziapi::config::Node &getServerConfig() const;
};
}
#endif //BASIC_SERVER_SERVER_HPP
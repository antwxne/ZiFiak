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
    const std::string &getPathDirectory() const;

private:
    Node _serverConfig;
    LoadLibs loadLibs;
public:
    const ziapi::config::Node &getServerConfig() const;
};
}
#endif //BASIC_SERVER_SERVER_HPP
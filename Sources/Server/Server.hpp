/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_SERVER_HPP
#define BASIC_SERVER_SERVER_HPP

#include "ziapi/Config.hpp"

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

private:
    Node _serverConfig;
};
}
#endif //BASIC_SERVER_SERVER_HPP

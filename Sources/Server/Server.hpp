/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_SERVER_HPP
#define BASIC_SERVER_SERVER_HPP

#include "nlohmann/json.hpp"

namespace zia::server {
class Server {
public:
    Server();
    ~Server() = default;
    Server(const Server &) = delete;
    Server(const Server &&) = delete;
    Server &operator=(const Server &) = delete;
    void init(const nlohmann::json &config);
    void run();

private:

};
}
#endif //BASIC_SERVER_SERVER_HPP

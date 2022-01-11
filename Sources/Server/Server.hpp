/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_SERVER_HPP
#define BASIC_SERVER_SERVER_HPP

#include <asio.hpp>
#include <vector>
#include <memory>

#include <Server/Client.hpp>

namespace Server_n {

class Server {
public:
    Server(unsigned short port = 8080);
    ~Server() = default;

    void run();

private:
    asio::io_context _io_context;
    asio::ip::tcp::acceptor _acceptor;
    std::vector<std::shared_ptr<Server_n::Client>> _clients;
    std::vector<uint8_t> _buffer;
};

}
#endif //BASIC_SERVER_SERVER_HPP

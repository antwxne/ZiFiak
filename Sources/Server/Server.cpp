/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "Server.hpp"

Server_n::Server::Server(unsigned short port)
    : _io_context(),
    _acceptor(_io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
    _clients(), _buffer(1 << 10)
{
}

void Server_n::Server::run()
{
    Debug::log("server running");
    _io_context.run();
}

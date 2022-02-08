/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "Server.hpp"

zia::server::Server::Server()
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->stop();
    });
}

void zia::server::Server::init(const nlohmann::json &config)
{
    Debug::log("init server");

    asio::ip::tcp::endpoint basicEndPoint(
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(),
            config["default"]["port"]));

    _internConfig = config;

    _acceptor.open(basicEndPoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(basicEndPoint);
    _acceptor.listen();
    startAccept();
}

void zia::server::Server::run()
{
    Debug::log("server running");
    _io_context.run();
}

void zia::server::Server::stop()
{
    Debug::log("server stop");
    _io_context.stop();
}

void zia::server::Server::startAccept()
{
    _clients.emplace_back(std::make_unique<zia::server::Client>(
        Client(_internConfig["max_request_size"], _io_context)));
    _acceptor.async_accept(_clients.back()->getAsioSocket(),
        std::bind(&Server::handleAccept, this,
            std::ref(*_clients.back().get())));
}

void zia::server::Server::startAcceptSSL()
{
    _clients.emplace_back(std::make_unique<zia::server::Client>(
        Client(_internConfig["max_request_size"], _io_context)));
    _acceptor.async_accept(_clients.back()->getAsioSocket(),
        std::bind(&Server::handleAcceptSSL, this,
            std::ref(*_clients.back().get())));
}

void zia::server::Server::handleAccept(zia::server::Client &client)
{
    Debug::log("Client connected");
    startReceive(client);
    startAccept();
}

void zia::server::Server::handleAcceptSSL(zia::server::Client &client)
{
    Debug::log("Client connected SSL");
    startReceiveSSL(client);
    startAcceptSSL();
}

void zia::server::Server::startReceive(zia::server::Client &client)
{
    client.getAsioSocket().async_receive(
        asio::buffer(client.getRawRequest(), _internConfig["max_request_size"]),
        std::bind(&Server::handleReceive, this, std::ref(client)));
}

void zia::server::Server::startReceiveSSL(zia::server::Client &client)
{
    client.getAsioSocket().async_receive(
        asio::buffer(client.getRawRequest(), _internConfig["max_request_size"]),
        std::bind(&Server::handleReceiveSSL, this, std::ref(client)));
}

void zia::server::Server::handleReceive(zia::server::Client &client)
{
    client.setProcessingARequest(true);
    // c'est ici qu'on gere la requete avec les modules etc

    client.setProcessingARequest(false);
    startReceive(client);
}

void zia::server::Server::handleReceiveSSL(zia::server::Client &client)
{
    client.setProcessingARequest(true);
    // c'est ici qu'on gere la requete SSL avec les modules etc

    client.setProcessingARequest(false);
    startReceiveSSL(client);
}

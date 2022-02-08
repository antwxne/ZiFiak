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

#include "nlohmann/json.hpp"
#include "Modules/Network/Client.hpp"


// TODO le core
// TODO [Thread] une fonction qui met a jour la config toute les x secondes(le temps doit etre set dans le fichier de conf)
// TODO [Thread] une fonction qui delete les clients inactif toutes les x secondes (le temps doit etre set dans le fichier de conf)

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
    void stop();

private:
    void startAccept();
    void handleAccept(Client &client);
    void startReceive(Client &client);
    void handleReceive(Client &client);

private:
    asio::io_context _io_context;
    asio::ip::tcp::acceptor _acceptor;
    asio::signal_set _signalSet;
    std::vector<std::unique_ptr<zia::server::Client>> _clients;
    std::vector<uint8_t> _buffer;
    nlohmann::json _internConfig;
};
}
#endif //BASIC_SERVER_SERVER_HPP

/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_BASICNETWORK_HPP
#define ZIA_BASICNETWORK_HPP

#include <asio.hpp>

#include "Client.hpp"
#include "ziapi/Module.hpp"

namespace zia::modules::network {
class BasicNetwork : public ziapi::INetworkModule {
public:
    BasicNetwork();
    ~BasicNetwork();
    // IModule
    void Init(const ziapi::config::Node &cfg) override;
    ziapi::Version GetVersion() const noexcept override;
    ziapi::Version GetCompatibleApiVersion() const noexcept override;
    const char *GetName() const noexcept override;
    const char *GetDescription() const noexcept override;
    // INetworkModule
    void Run(ziapi::http::IRequestOutputQueue &requests,
        ziapi::http::IResponseInputQueue &responses
    ) override;
    void Terminate() override;

private:
    void startAccept(ziapi::http::IRequestOutputQueue &requests);
    void handleAccept(ziapi::http::IRequestOutputQueue &requests,
        Client &client
    );
    void startReceive(ziapi::http::IRequestOutputQueue &requests,
        Client &client
    );
    void handleReceive(ziapi::http::IRequestOutputQueue &requests,
        Client &client, const std::error_code &error,
        std::size_t bytes_transfered
    );
    void sendResponses(ziapi::http::IResponseInputQueue &responses);
    void disconnectClient() noexcept;

private:
    asio::io_context _io_context;
    asio::ip::tcp::acceptor _acceptor;
    asio::signal_set _signalSet;
    bool _isRunning;
    std::vector<std::unique_ptr<Client>> _clients;
    std::thread _thread;
};
}

#endif //ZIA_BASICNETWORK_HPP

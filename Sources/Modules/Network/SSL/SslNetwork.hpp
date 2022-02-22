/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_SSLNETWORK_HPP
#define ZIA_SSLNETWORK_HPP

#include <asio/ssl.hpp>

#include "ziapi/Module.hpp"
#include "ziapi/Http.hpp"

#include "SSLClient.hpp"

namespace zia::modules::network {
class SSLNetwork: public ziapi::INetworkModule {
public:
    SSLNetwork();
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
        SSLClient &client
    );
    void startReceive(ziapi::http::IRequestOutputQueue &requests,
        SSLClient &client
    );
    void handleReceive(ziapi::http::IRequestOutputQueue &requests,
        SSLClient &client, const std::error_code &error,
        std::size_t bytes_transfered
    );
    void sendResponses(ziapi::http::IResponseInputQueue &responses);

private:
    asio::io_context _io_context;
    asio::ip::tcp::acceptor _acceptor;
    asio::signal_set _signalSet;
    bool _isRunning;
    std::vector<std::unique_ptr<SSLClient>> _clients;
    std::thread _responseThread;
    asio::ssl::context _sslContext;
};
}
#endif //ZIA_SSLNETWORK_HPP

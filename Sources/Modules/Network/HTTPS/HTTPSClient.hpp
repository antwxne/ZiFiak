/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_CLIENT_HPP
#define BASIC_SERVER_CLIENT_HPP

#include <asio/ssl.hpp>
#include <memory>

#include "Modules/Network/AClient.hpp"

namespace zia::modules::network {
class HTTPSClient : public AClient {
public:
    HTTPSClient(asio::io_context &ioContext, asio::ssl::context& sslContext);
    int getSocketFd() override;
    bool operator==(int fd) noexcept override;
    bool operator==(const ziapi::http::Context &ctx) const override;
    [[nodiscard]] ziapi::http::Context getContext() const noexcept override;
    const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> &getAsioSSLSocket() const noexcept;
    asio::ip::tcp::socket &getAsioSocket() noexcept;
    void initSSL();
    asio::io_context::strand &getStrand();
private:
    HTTPSClient &genericSend(const void *obj, const std::size_t &size) override;

private:
    asio::ip::tcp::socket _socket;
    std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> _sslSocket;
    asio::ssl::context &_sslContext;
    asio::io_context::strand _strand;
};
}

#endif //BASIC_SERVER_CLIENT_HPP

/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_CLIENT_HPP
#define BASIC_SERVER_CLIENT_HPP

#include "Modules/Network/AClient.hpp"

namespace zia::modules::network {
class HTTPClient : public AClient {
public:
    HTTPClient(asio::io_context &ioContext);
    int getSocketFd() override;
    bool operator==(int fd) noexcept override;
    bool operator==(const ziapi::http::Context &ctx) const override;
    [[nodiscard]] ziapi::http::Context getContext() const noexcept override;
    asio::ip::tcp::socket &getAsioSocket() noexcept;
private:
    HTTPClient &genericSend(const void *obj, const std::size_t &size) override;

private:
    asio::ip::tcp::socket _socket;
};
}

#endif //BASIC_SERVER_CLIENT_HPP

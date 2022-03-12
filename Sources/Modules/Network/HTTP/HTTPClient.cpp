/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "HTTPClient.hpp"

zia::modules::network::HTTPClient::HTTPClient(
    asio::io_context &ioContext
) : AClient(), _socket(ioContext)
{
    if (_socket.native_handle() == -1) {
        throw std::runtime_error("Corrupted or Bad file descriptor");
    }
}


int zia::modules::network::HTTPClient::getSocketFd()
{
    return _socket.native_handle();
}


ziapi::http::Context zia::modules::network::HTTPClient::getContext() noexcept
{
    ziapi::http::Context ctx;
    asio::error_code ec;

    ctx["client.socket.address"] = _socket.remote_endpoint(ec).address().to_string();
    if (ec) {
        _isNew = false;
        _isConnected = false;
    }
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_socket.remote_endpoint(ec).port());
    if (ec) {
        _isNew = false;
        _isConnected = false;
    }
    return ctx;
}

bool zia::modules::network::HTTPClient::operator==(const ziapi::http::Context &ctx
) const
{
    bool dest = true;

    dest &= this->_socket.remote_endpoint().address().to_string() ==
        std::any_cast<std::string>(ctx.at("client.socket.address"));
    dest &= this->_socket.remote_endpoint().port() ==
        std::any_cast<std::uint16_t>(ctx.at("client.socket.port"));
    return dest;
}

bool zia::modules::network::HTTPClient::operator==(int fd) noexcept
{
    return fd == getSocketFd();
}

asio::ip::tcp::socket &zia::modules::network::HTTPClient::getAsioSocket() noexcept
{
    return _socket;
}

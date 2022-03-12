/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "HTTPSClient.hpp"

zia::modules::network::HTTPSClient::HTTPSClient(asio::io_context &ioContext,
    asio::ssl::context &sslContext
) : AClient(), _socket(ioContext), _sslContext(sslContext), _strand(ioContext)
{
    if (_socket.native_handle() == -1) {
        throw std::runtime_error("Corrupted or Bad file descriptor");
    }
}

void zia::modules::network::HTTPSClient::initSSL()
{
    _sslSocket = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(
        std::move(_socket), _sslContext);
}

int zia::modules::network::HTTPSClient::getSocketFd()
{
    return _sslSocket->lowest_layer().native_handle();
}

ziapi::http::Context zia::modules::network::HTTPSClient::getContext() noexcept
{
    ziapi::http::Context ctx;
    asio::error_code ec;

    if (_sslSocket == nullptr) {
        return ctx;
    }
    ctx["client.socket.address"] = _sslSocket->lowest_layer().remote_endpoint(ec).address().to_string();
    if (ec) {
        _isNew = false;
        _isConnected = false;
    }
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_sslSocket->lowest_layer().remote_endpoint(ec).port());
    if (ec) {
        _isNew = false;
        _isConnected = false;
    }
    return ctx;
}

bool zia::modules::network::HTTPSClient::operator==(
    const ziapi::http::Context &ctx
) const
{
    bool dest = true;

    if (_sslSocket == nullptr) {
        return false;
    }
    try {
        dest &=
            this->_sslSocket->lowest_layer().remote_endpoint().address().to_string() ==
                std::any_cast<std::string>(ctx.at("client.socket.address"));
        dest &= this->_sslSocket->lowest_layer().remote_endpoint().port() ==
            std::any_cast<std::uint16_t>(ctx.at("client.socket.port"));
    } catch(...) {
        return false;
    }
    return dest;
}

bool zia::modules::network::HTTPSClient::operator==(int fd) noexcept
{
    return fd == getSocketFd();
}


asio::ip::tcp::socket &zia::modules::network::HTTPSClient::getAsioSocket() noexcept
{
    return _socket;
}

const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> &zia::modules::network::HTTPSClient::getAsioSSLSocket() const noexcept
{
    return _sslSocket;
}

asio::io_context::strand &zia::modules::network::HTTPSClient::getStrand()
{
    return _strand;
}



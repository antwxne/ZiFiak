/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "SSLClient.hpp"

zia::modules::network::SSLClient::SSLClient(
    asio::io_context &ioContext, asio::ssl::context &sslContext
) : AClient(), _socket(ioContext), _sslContext(sslContext)
{
}

int zia::modules::network::SSLClient::getSocketFd()
{
    return _sslSocket->lowest_layer().native_handle();
}

ziapi::http::Context zia::modules::network::SSLClient::getContext() const noexcept
{
    ziapi::http::Context ctx;

    ctx["client.socket.address"] = _sslSocket->lowest_layer().remote_endpoint().address();
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_sslSocket->lowest_layer().remote_endpoint().port());
    return ctx;
}

bool zia::modules::network::SSLClient::operator==(
    const ziapi::http::Context &ctx
) const
{
    bool dest = true;

    dest &= this->_sslSocket->lowest_layer().remote_endpoint().address().to_string() ==
        std::any_cast<std::string>(ctx.at("client.socket.address"));
    dest &= this->_sslSocket->lowest_layer().remote_endpoint().port() ==
        std::any_cast<std::uint16_t>(ctx.at("client.socket.port"));
    return dest;
}

bool zia::modules::network::SSLClient::operator==(int fd) noexcept
{
    return fd == getSocketFd();
}

zia::modules::network::SSLClient &zia::modules::network::SSLClient::genericSend(
    const void *obj, const std::size_t &size
)
{
    this->_socket.async_send(asio::buffer(obj, size),
        [](const asio::error_code &errorCode, std::size_t bytesTransferred) {
            if (errorCode) {
                throw MyException(errorCode.message(), __PRETTY_FUNCTION__,
                    __FILE__, __LINE__);
            }
            Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
        });
    return *this;
}

asio::ip::tcp::socket &zia::modules::network::SSLClient::getAsioSocket() noexcept
{
    return _socket;
}

const std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> &zia::modules::network::SSLClient::getAsioSSLSocket() const noexcept
{
    return _sslSocket;
}

void zia::modules::network::SSLClient::initSSL()
{
    _sslSocket = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(
            std::move(_socket), _sslContext);
}

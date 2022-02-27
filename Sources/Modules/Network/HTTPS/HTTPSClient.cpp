/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "HTTPSClient.hpp"

zia::modules::network::HTTPSClient::HTTPSClient(
    asio::io_context &ioContext, asio::ssl::context &sslContext
) : AClient(), _socket(ioContext), _sslContext(sslContext), _strand(ioContext)
{
}

void zia::modules::network::HTTPSClient::initSSL()
{
    _sslSocket = std::make_shared<asio::ssl::stream<asio::ip::tcp::socket>>(
        std::move(_socket), _sslContext);

//    _sslSocket->async_handshake(asio::ssl::stream_base::server,
//        [this](const std::error_code& error) {
//            if (error) {
//                this->_isConnected = false;
//                throw std::runtime_error("Error handshake");
//            }
//    });
}

int zia::modules::network::HTTPSClient::getSocketFd()
{
    return _sslSocket->lowest_layer().native_handle();
}

ziapi::http::Context zia::modules::network::HTTPSClient::getContext() const noexcept
{
    ziapi::http::Context ctx;

    ctx["client.socket.address"] = _sslSocket->lowest_layer().remote_endpoint().address().to_string();
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_sslSocket->lowest_layer().remote_endpoint().port());
    return ctx;
}

bool zia::modules::network::HTTPSClient::operator==(
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

bool zia::modules::network::HTTPSClient::operator==(int fd) noexcept
{
    return fd == getSocketFd();
}

zia::modules::network::HTTPSClient &zia::modules::network::HTTPSClient::genericSend(
    const void *obj, const std::size_t &size
)
{
    this->_socket.async_send(asio::buffer(obj, size),
        [this](const asio::error_code &errorCode, std::size_t bytesTransferred) {
            if (errorCode) {
                throw MyException(errorCode.message(), __PRETTY_FUNCTION__,
                    __FILE__, __LINE__);
            }
            if (!this->_keepAlive) {
                this->_isConnected = false;
            }
            this->updateTime();
            this->_processingRequest = false;
            Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
        });
    return *this;
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



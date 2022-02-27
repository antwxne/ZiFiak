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
}


int zia::modules::network::HTTPClient::getSocketFd()
{
    return _socket.native_handle();
}


ziapi::http::Context zia::modules::network::HTTPClient::getContext() const noexcept
{
    ziapi::http::Context ctx;

    ctx["client.socket.address"] = _socket.remote_endpoint().address().to_string();
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_socket.remote_endpoint().port());
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

zia::modules::network::HTTPClient &zia::modules::network::HTTPClient::genericSend(
    const void *obj, const std::size_t &size
)
{
    this->_socket.async_send(asio::buffer(obj, size),
        [this](const asio::error_code &errorCode, std::size_t bytesTransferred) {
            if (errorCode) {
                throw MyException(errorCode.message(), __PRETTY_FUNCTION__,
                    __FILE__, __LINE__);
            }
            if (!this->_keepAlive.has_value()) {
                this->_isConnected = false;
                this->_processingRequest = false;
            } else {
                this->_keepAlive->max -= 1;
            }
            this->updateTime();
            Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
        });
    return *this;
}

asio::ip::tcp::socket &zia::modules::network::HTTPClient::getAsioSocket() noexcept
{
    return _socket;
}

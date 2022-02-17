/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "SSLClient.hpp"

zia::modules::network::SSLClient::SSLClient(const size_t &bufferSize,
    asio::io_context &ioContext
) : AClient(bufferSize), _socket(ioContext)
{
}


int zia::modules::network::SSLClient::getSocketFd()
{
    return _socket.native_handle();
}


ziapi::http::Context zia::modules::network::SSLClient::getContext() const noexcept
{
    ziapi::http::Context ctx;

    ctx["client.socket.address"] = _socket.remote_endpoint().address().to_string();
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_socket.remote_endpoint().port());
    return ctx;
}

bool zia::modules::network::SSLClient::operator==(const ziapi::http::Context &ctx
) const
{
    bool dest = true;

    dest &= this->_socket.remote_endpoint().address().to_string() ==
        std::any_cast<std::string>(ctx.at("client.socket.address"));
    dest &= this->_socket.remote_endpoint().port() ==
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
/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "Client.hpp"

zia::server::Client::Client(const size_t &bufferSize,
    asio::io_context &ioContext
) : _socket(ioContext), _keepAlive(false), _processingRequest(false),
    _rawRequest(bufferSize, 0), _lastRequest(std::chrono::system_clock::now())
{
}

asio::ip::tcp::socket &zia::server::Client::getAsioSocket()
{
    return _socket;
}

int zia::server::Client::getSocketFd()
{
    return _socket.native_handle();
}

const std::vector<uint8_t> &zia::server::Client::getRawRequest() const noexcept
{
    return _rawRequest;
}

std::vector<uint8_t> &zia::server::Client::getRawRequest() noexcept
{
    return _rawRequest;
}

bool zia::server::Client::isProcessingARequest() const noexcept
{
    return _processingRequest;
}

bool zia::server::Client::isKeepingAlive() const noexcept
{
    return _keepAlive;
}

void zia::server::Client::setProcessingARequest(bool var) noexcept
{
    _processingRequest = var;
}

void zia::server::Client::setKeepAlive(bool var) noexcept
{
    _keepAlive = var;
}

const std::chrono::time_point<std::chrono::system_clock> &zia::server::Client::getTimeLastRequest() const noexcept
{
    return _lastRequest;
}

void zia::server::Client::updateTime() noexcept
{
    _lastRequest = std::chrono::system_clock::now();
}

void zia::server::Client::changeBufferSize(const std::size_t &size) noexcept
{
    _rawRequest.resize(size);
}

zia::server::Client &zia::server::Client::operator<<(std::string &str)
{
    return genericSend(str.c_str(), str.size());
}

zia::server::Client &zia::server::Client::operator<<(std::vector<uint8_t> &arr)
{
    return genericSend(&*arr.begin(), arr.size());
}

void zia::server::Client::operator>>(std::string &str) const
{
    str.resize(_rawRequest.size());
    std::memcpy(&*str.begin(),&*_rawRequest.begin(), _rawRequest.size());
}

void zia::server::Client::operator>>(std::vector<uint8_t> &arr) const
{
    arr.resize(_rawRequest.size());
    std::memcpy(&*arr.begin(), &*_rawRequest.begin(), _rawRequest.size());
}

zia::server::Client &zia::server::Client::genericSend(const void *obj,
    const std::size_t &size
)
{
    // on peut changer la callback par un module post request
    this->_socket.async_send(asio::buffer(obj, size),
        [](const asio::error_code &errorCode, std::size_t bytesTransferred) {
            if (errorCode) {
                throw MyException(errorCode.message(), __PRETTY_FUNCTION__, __FILE__, __LINE__);
            }
            Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
        });
    return *this;
}

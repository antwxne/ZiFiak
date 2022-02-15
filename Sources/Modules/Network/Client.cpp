/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include "Client.hpp"

zia::modules::network::Client::Client(const size_t &bufferSize,
    asio::io_context &ioContext
) : _socket(ioContext), _keepAlive(false), _processingRequest(false),
    _rawRequest(bufferSize, 0), _lastRequest(std::chrono::system_clock::now())
{
}

asio::ip::tcp::socket &zia::modules::network::Client::getAsioSocket()
{
    return _socket;
}

int zia::modules::network::Client::getSocketFd()
{
    return _socket.native_handle();
}

const std::vector<uint8_t> &zia::modules::network::Client::getRawRequest() const noexcept
{
    return _rawRequest;
}

std::vector<uint8_t> &zia::modules::network::Client::getRawRequest() noexcept
{
    return _rawRequest;
}

bool zia::modules::network::Client::isProcessingARequest() const noexcept
{
    return _processingRequest;
}

bool zia::modules::network::Client::isKeepingAlive() const noexcept
{
    return _keepAlive;
}

void zia::modules::network::Client::setProcessingARequest(bool var) noexcept
{
    _processingRequest = var;
}

void zia::modules::network::Client::setKeepAlive(bool var) noexcept
{
    _keepAlive = var;
}

const std::chrono::time_point<std::chrono::system_clock> &zia::modules::network::Client::getTimeLastRequest() const noexcept
{
    return _lastRequest;
}

void zia::modules::network::Client::updateTime() noexcept
{
    _lastRequest = std::chrono::system_clock::now();
}

void zia::modules::network::Client::changeBufferSize(const std::size_t &size
) noexcept
{
    _rawRequest.resize(size);
}

zia::modules::network::Client &zia::modules::network::Client::operator<<(
    std::string &str
)
{
    return genericSend(str.c_str(), str.size());
}

zia::modules::network::Client &zia::modules::network::Client::operator<<(
    std::vector<uint8_t> &arr
)
{
    return genericSend(&*arr.begin(), arr.size());
}

void zia::modules::network::Client::operator>>(std::string &str) const
{
    str.resize(_rawRequest.size());
    std::memcpy(&*str.begin(), &*_rawRequest.begin(), _rawRequest.size());
}

void zia::modules::network::Client::operator>>(std::vector<uint8_t> &arr) const
{
    arr.resize(_rawRequest.size());
    std::memcpy(&*arr.begin(), &*_rawRequest.begin(), _rawRequest.size());
}

zia::modules::network::Client &zia::modules::network::Client::genericSend(
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

zia::modules::network::Client &zia::modules::network::Client::operator+=(
    const std::vector<uint8_t> &arr
)
{
    _rawRequest.end();

    _rawRequest.insert(_rawRequest.begin(), arr.cbegin(), arr.cend());
    return *this;
}

bool zia::modules::network::Client::isConnected() const
{
    return _isConnected;
}

void zia::modules::network::Client::setConnectionStatut(bool isConnected)
{
    _isConnected = isConnected;
}

zia::modules::network::Client &zia::modules::network::Client::operator<<(
    const ziapi::http::Response &response
)
{
    std::string res = "";
    res += std::to_string(static_cast<int>(response.version)) + "\r\n" +
        std::to_string(static_cast<int>(response.status_code)) + "\r\n" +
        response.reason + "\r\n";

    for (const auto &value: response.fields) {
        res += value.first + ": " + value.second + "\r\n";
    }
    res += response.body + "\r\n";
    return genericSend(&*res.begin(), res.size());
}

ziapi::http::Context zia::modules::network::Client::getContext() const noexcept
{
    ziapi::http::Context ctx;

    ctx["client.socket.address"] = _socket.remote_endpoint().address().to_string();
    ctx["client.socket.port"] = static_cast<std::uint16_t>(_socket.remote_endpoint().port());
    return ctx;
}

bool zia::modules::network::Client::operator==(const ziapi::http::Context &ctx
) const
{
    bool dest = true;

    dest &= this->_socket.remote_endpoint().address().to_string() ==
        std::any_cast<std::string>(ctx.at("client.socket.address"));
    dest &= this->_socket.remote_endpoint().port() ==
        std::any_cast<std::uint16_t>(ctx.at("client.socket.port"));
    return dest;
}

bool zia::modules::network::Client::operator==(int fd)
{
    return fd == getSocketFd();
}

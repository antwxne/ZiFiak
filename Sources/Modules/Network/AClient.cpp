/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "Modules/Http/HttpModule.hpp"
#include "AClient.hpp"

zia::modules::network::AClient::AClient() : _keepAlive(false), _processingRequest(false), _isConnected(true), _rawRequest(),
    _lastRequest(std::chrono::system_clock::now())
{
}

const std::vector<uint8_t> &zia::modules::network::AClient::getRawRequest() const noexcept
{
    return _rawRequest;
}

std::vector<uint8_t> &zia::modules::network::AClient::getRawRequest() noexcept
{
    return _rawRequest;
}

bool zia::modules::network::AClient::isProcessingARequest() const noexcept
{
    return _processingRequest;
}

bool zia::modules::network::AClient::isKeepingAlive() const noexcept
{
    return _keepAlive;
}

void zia::modules::network::AClient::setProcessingARequest(bool var) noexcept
{
    _processingRequest = var;
}

void zia::modules::network::AClient::setKeepAlive(bool var) noexcept
{
    _keepAlive = var;
}

const std::chrono::time_point<std::chrono::system_clock> &zia::modules::network::AClient::getTimeLastRequest() const noexcept
{
    return _lastRequest;
}

void zia::modules::network::AClient::updateTime() noexcept
{
    _lastRequest = std::chrono::system_clock::now();
}

void zia::modules::network::AClient::changeBufferSize(const std::size_t &size
) noexcept
{
    _rawRequest.resize(size);
}

zia::modules::network::AClient &zia::modules::network::AClient::operator<<(
    std::string &str
)
{
    return genericSend(str.c_str(), str.size() * sizeof (*str.c_str()));
}

zia::modules::network::AClient &zia::modules::network::AClient::operator<<(
    std::vector<uint8_t> &arr
)
{
    return genericSend(&*arr.begin(), arr.size() * sizeof(*arr.begin()));
}

void zia::modules::network::AClient::operator>>(std::string &str) const
{
    str.resize(_rawRequest.size()  * sizeof(*_rawRequest.begin()));
    std::memcpy(&*str.begin(), &*_rawRequest.begin(), _rawRequest.size() * sizeof(*_rawRequest.begin()));
}

void zia::modules::network::AClient::operator>>(std::vector<uint8_t> &arr) const
{
    arr.resize(_rawRequest.size() * sizeof(*_rawRequest.begin()));
    std::memcpy(&*arr.begin(), &*_rawRequest.begin(), _rawRequest.size() * sizeof(*_rawRequest.begin()));
}

zia::modules::network::AClient &zia::modules::network::AClient::operator+=(
    const std::vector<uint8_t> &arr
)
{
    _rawRequest.insert(_rawRequest.end(), arr.cbegin(), arr.cend());
    return *this;
}

bool zia::modules::network::AClient::isConnected() const
{
    return _isConnected;
}

void zia::modules::network::AClient::setConnectionStatut(bool isConnected)
{
    _isConnected = isConnected;
}

zia::modules::network::AClient &zia::modules::network::AClient::operator<<(
    const ziapi::http::Response &response
)
{
    std::string res = zia::modules::http::HttpModule::readResponse(response);
    return genericSend(res.c_str(), res.size() * sizeof(*res.c_str()));
}

std::string zia::modules::network::AClient::toString() const noexcept
{
    std::string dest;

    dest.resize(_rawRequest.size() * sizeof(*_rawRequest.begin()));

    std::memcpy(&*dest.begin(), &*_rawRequest.begin(),
        _rawRequest.size() * sizeof(*_rawRequest.begin()));
    return dest;
}

void zia::modules::network::AClient::empty()
{
    _rawRequest.clear();
}

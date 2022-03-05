/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "AClient.hpp"
#include "Modules/Network/HTTPParser/HTTPParser.hpp"

zia::modules::network::AClient::AClient() :
    _processingRequest(0), _isConnected(true), _rawRequest(),
    _lastRequest(std::chrono::steady_clock::now()), _keepAlive(std::nullopt), _isNew(true)
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

int zia::modules::network::AClient::isProcessingARequest() const noexcept
{
    return _processingRequest;
}

void zia::modules::network::AClient::IncrementProcessingARequest() noexcept
{
    _processingRequest++;
}

void zia::modules::network::AClient::setKeepAlive(
    const ziapi::http::Request &req
) noexcept
{
    try {
        const auto &connectionValue = req.headers.at(
            ziapi::http::header::kConnection);
        if (connectionValue == "keep-alive") {
            _keepAlive = KeepAliveInfos(true);
        } else {
            _keepAlive = KeepAliveInfos(false);
        }
    } catch (...) {
        if (!_keepAlive.has_value()) {
            _keepAlive = std::nullopt;
        }
    }
    try {
        const auto &keepAliveValue = req.headers.at(
            ziapi::http::header::kKeepAlive);
        auto values = zia::modules::network::HTTPParser::parseKeepAliveInfos(
            keepAliveValue);
        if (_keepAlive.has_value()) {
            _keepAlive = KeepAliveInfos(_keepAlive.value().always, values.first,
                values.second);
        } else {
            _keepAlive = KeepAliveInfos(false, values.first, values.second);
        }
    } catch (...) {
        if (!_keepAlive.has_value()) {
            _keepAlive = std::nullopt;
        }
    }
}

const std::chrono::steady_clock::time_point &zia::modules::network::AClient::getTimeLastRequest() const noexcept
{
    return _lastRequest;
}

void zia::modules::network::AClient::updateTime() noexcept
{
    _lastRequest = std::chrono::steady_clock::now();
}

void zia::modules::network::AClient::changeBufferSize(const std::size_t &size
) noexcept
{
    _rawRequest.resize(size);
}

void zia::modules::network::AClient::operator>>(std::string &str) const
{
    str.resize(_rawRequest.size() * sizeof(*_rawRequest.begin()));
    std::memcpy(&*str.begin(), &*_rawRequest.begin(),
        _rawRequest.size() * sizeof(*_rawRequest.begin()));
}

void zia::modules::network::AClient::operator>>(std::vector<uint8_t> &arr) const
{
    arr.resize(_rawRequest.size() * sizeof(*_rawRequest.begin()));
    std::memcpy(&*arr.begin(), &*_rawRequest.begin(),
        _rawRequest.size() * sizeof(*_rawRequest.begin()));
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

std::string zia::modules::network::AClient::toString() const noexcept
{
    std::string dest;

    dest.resize(_rawRequest.size() * sizeof(*_rawRequest.begin()));

    std::memcpy(&*dest.begin(), &*_rawRequest.begin(),
        _rawRequest.size() * sizeof(*_rawRequest.begin()));
    return dest;
}

void zia::modules::network::AClient::clearRawRequest()
{
    _rawRequest.clear();
}

std::vector<uint8_t> &zia::modules::network::AClient::getBuffer() noexcept
{
    return _buffer;
}

void zia::modules::network::AClient::saveBuffer() noexcept
{
    _rawRequest.insert(_rawRequest.cend(), _buffer.begin(), _buffer.end());
}

void zia::modules::network::AClient::clearBuffer() noexcept
{
    _buffer.clear();
}

const std::optional<zia::modules::network::KeepAliveInfos> &zia::modules::network::AClient::getKeepAliveInfos() const noexcept
{
    return _keepAlive;
}

void zia::modules::network::AClient::DecrementProcessingARequest() noexcept
{
    _processingRequest--;
    _isNew = false;
}

bool zia::modules::network::AClient::isNewClient() const noexcept
{
    return _isNew;
}

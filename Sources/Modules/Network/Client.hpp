/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_CLIENT_HPP
#define BASIC_SERVER_CLIENT_HPP

#include <asio.hpp>
#include <vector>
#include <iostream>
#include <chrono>

#include "Exceptions/MyException.hpp"
#include "Debug/Debug.hpp"

namespace zia::modules::network {
class Client {
public:
    Client(const std::size_t &bufferSize, asio::io_context &ioContext);

    template<typename T>
    Client &operator<<(const T &obj)
    {
        return genericSend(&obj, sizeof obj);
    }

    Client &operator<<(std::string &str);
    Client &operator<<(std::vector<uint8_t> &arr);
    void operator>>(std::string &str) const;
    void operator>>(std::vector<uint8_t> &arr) const;
    asio::ip::tcp::socket &getAsioSocket();
    int getSocketFd();
    const std::vector<uint8_t> &getRawRequest() const noexcept;
    std::vector<uint8_t> &getRawRequest() noexcept;
    bool isProcessingARequest() const noexcept;
    bool isKeepingAlive() const noexcept;
    void setProcessingARequest(bool var) noexcept;
    void setKeepAlive(bool var) noexcept;
    const std::chrono::time_point<std::chrono::system_clock> &getTimeLastRequest() const noexcept;
    void updateTime() noexcept;
    void changeBufferSize(const std::size_t &newSize) noexcept;

private:
    Client &genericSend(const void *obj, const std::size_t &size);

private:
    asio::ip::tcp::socket _socket;
    bool _keepAlive;
    bool _processingRequest;
    std::vector<uint8_t> _rawRequest;
    std::chrono::time_point<std::chrono::system_clock> _lastRequest;
};
}

#endif //BASIC_SERVER_CLIENT_HPP

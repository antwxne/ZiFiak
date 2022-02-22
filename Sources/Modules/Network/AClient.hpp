/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_ACLIENT_HPP
#define ZIA_ACLIENT_HPP

#include <asio.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include <optional>

#include "ziapi/Http.hpp"
#include "Exceptions/MyException.hpp"
#include "Debug/Debug.hpp"

namespace zia::modules::network {

struct KeepAliveInfos {
    KeepAliveInfos(int vtimeout, int vmax): timeout(vtimeout), max(vmax) {}
    KeepAliveInfos(const KeepAliveInfos &other)
    {
        timeout = other.timeout;
        max = other.timeout;
    }
    int timeout;
    int max;
};

class AClient {
public:
    AClient();

    template<typename T>
    AClient &operator<<(const T &obj)
    {
        return genericSend(&obj, sizeof obj);
    }

    AClient &operator<<(std::string &str);
    AClient &operator<<(std::vector <uint8_t> &arr);
    AClient &operator<<(const ziapi::http::Response &response);
    void operator>>(std::string &str) const;
    void operator>>(std::vector <uint8_t> &arr) const;
    AClient &operator+=(const std::vector <uint8_t> &arr);

    [[nodiscard]] const std::vector <uint8_t> &getRawRequest() const noexcept;
    std::vector <uint8_t> &getRawRequest() noexcept;
    [[nodiscard]] bool isProcessingARequest() const noexcept;
    void setProcessingARequest(bool var) noexcept;
    void setKeepAlive(const ziapi::http::Request &req) noexcept;
    [[nodiscard]] const std::optional<KeepAliveInfos> &getKeepAliveInfos() const noexcept;
    [[nodiscard]] const std::chrono::steady_clock::time_point &getTimeLastRequest() const noexcept;
    void updateTime() noexcept;
    void changeBufferSize(const std::size_t &newSize) noexcept;
    [[nodiscard]] bool isConnected() const;
    void setConnectionStatut(bool isConnected);
    [[nodiscard]] std::string toString() const noexcept;
    void clearRawRequest();
    std::vector<uint8_t> &getBuffer() noexcept;
    void saveBuffer() noexcept;
    void clearBuffer() noexcept;

public:
    virtual int getSocketFd() = 0;
    virtual bool operator==(int fd) noexcept = 0;
    virtual bool operator==(const ziapi::http::Context &ctx) const = 0;
    [[nodiscard]] virtual ziapi::http::Context getContext() const noexcept = 0;
protected:

    virtual AClient &genericSend(const void *obj, const std::size_t &size) = 0;
protected:
    bool _processingRequest;
    bool _isConnected;
    std::vector <uint8_t> _rawRequest;
    std::vector <uint8_t> _buffer;
    std::chrono::steady_clock::time_point _lastRequest;
    std::optional<KeepAliveInfos> _keepAlive;
};
}
#endif //ZIA_ACLIENT_HPP

/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_ICONTEXT_HPP
#define ZIA_ICONTEXT_HPP

#include <vector>
#include <cstdint>

class IContext {
public:
    virtual ~IContext() = default;

    virtual int getSocketFd() const noexcept = 0;
    virtual const std::vector<uint8_t> &getRawBuffer() const noexcept = 0;
    virtual std::vector<uint8_t> &getRawResponse() const noexcept = 0;
    virtual void setRawBuffer(const std::vector<uint8_t> & buff) = 0;
    virtual void setRawResponse(std::vector<uint8_t> &response) = 0;
    virtual void setSocketFd(int fd) const noexcept = 0;
};

#endif //ZIA_ICONTEXT_HPP

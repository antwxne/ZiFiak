/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_REQUESTQUEUE_HPP
#define ZIA_REQUESTQUEUE_HPP

#include <vector>
#include <mutex>

#include "ziapi/Http.hpp"

namespace zia::container {
class RequestQueue
    : public std::vector<std::pair<ziapi::http::Request, ziapi::http::Context>>,
        public ziapi::http::IRequestOutputQueue {
public:
    RequestQueue() = default;
    ~RequestQueue() override = default;

    void Push(std::pair<ziapi::http::Request, ziapi::http::Context> &&req) override;
    [[nodiscard]] std::size_t Size() const noexcept override;

private:
    std::mutex _mutex;
};
}

#endif //ZIA_REQUESTQUEUE_HPP

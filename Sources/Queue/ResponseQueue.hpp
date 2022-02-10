/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_RESPONSEQUEUE_HPP
#define ZIA_RESPONSEQUEUE_HPP

#include <vector>

#include "ziapi/Http.hpp"

namespace zia::container {
class ResponseQueue
    : public std::vector<std::pair<ziapi::http::Response, ziapi::http::Context>>,
        public ziapi::http::IResponseInputQueue {
public:
    ResponseQueue() = default;
    ~ResponseQueue() override = default;

    std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>> Pop() override;
    [[nodiscard]] std::size_t Size() const noexcept override;
};
}
#endif //ZIA_RESPONSEQUEUE_HPP

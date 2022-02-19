/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "ResponseQueue.hpp"

std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>> zia::container::ResponseQueue::Pop()
{
    _mutex.lock();
    if (!empty()) {
        std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>> dest = *begin();

        erase(cbegin());
        _mutex.unlock();
        return dest;
    }
    _mutex.unlock();
    return std::nullopt;
}

std::size_t zia::container::ResponseQueue::Size() const noexcept
{
    return size();
}

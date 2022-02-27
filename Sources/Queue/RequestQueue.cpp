/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "RequestQueue.hpp"

void zia::container::RequestQueue::Push(
    std::pair<ziapi::http::Request, ziapi::http::Context> &&req
)
{
    _mutex.lock();
    push_back(req);
    _mutex.unlock();
}

std::size_t zia::container::RequestQueue::Size() const noexcept
{
    return size();
}

std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>> zia::container::RequestQueue::Pop()
{
    _mutex.lock();
    if (!empty()) {
        std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>> dest = *begin();

        erase(cbegin());
        _mutex.unlock();
        return dest;
    }
    _mutex.unlock();
    return std::nullopt;
}


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

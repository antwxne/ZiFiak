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
    push_back(req);
}

std::size_t zia::container::RequestQueue::Size() const noexcept
{
    return size();
}

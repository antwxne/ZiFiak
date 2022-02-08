/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "BasicNetwork.hpp"

zia::modules::network::BasicNetwork::BasicNetwork(): _io_context(), _acceptor(_io_context), _signalSet(_io_context)
{
}

void zia::modules::network::BasicNetwork::Init(const ziapi::config::Node &cfg)
{
}

ziapi::Version zia::modules::network::BasicNetwork::GetVersion() const noexcept
{
    return {0, 0};
}

ziapi::Version zia::modules::network::BasicNetwork::GetCompatibleApiVersion() const noexcept
{
    return {0, 0};
}

const char *zia::modules::network::BasicNetwork::GetName() const noexcept
{
    return "Basic network module";
}

const char *zia::modules::network::BasicNetwork::GetDescription() const noexcept
{
    return "je suis un gentil petit module de merde";
}

void zia::modules::network::BasicNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses
)
{
}

void zia::modules::network::BasicNetwork::Terminate()
{
}

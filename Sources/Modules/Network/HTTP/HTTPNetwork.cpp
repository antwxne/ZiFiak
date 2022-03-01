/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include <thread>

#include "HTTPNetwork.hpp"
#include "Debug/Debug.hpp"
#include "Modules/Network/HTTPParser/HTTPParser.hpp"

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new zia::modules::network::HTTPNetwork;
}

zia::modules::network::HTTPNetwork::HTTPNetwork()
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context), _init(false)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->Terminate();
    });
}

zia::modules::network::HTTPNetwork::~HTTPNetwork()
{
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    _io_context.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
    Debug::log("HTTP network module Destroyed");
}

void zia::modules::network::HTTPNetwork::Init(const ziapi::config::Node &cfg)
{
    Debug::log("Init HTTP network module...");

    try {
        int port = cfg["http"]["port"].AsInt();
        asio::ip::tcp::endpoint basicEndPoint(
            asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

        _acceptor.open(basicEndPoint.protocol());
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(basicEndPoint);
        _acceptor.listen();
        _init = true;
    } catch (const std::out_of_range &e) {
        Debug::err("Can't init HTTP module, please check the config file");
        _init = false;
        Terminate();
    }

}

ziapi::Version zia::modules::network::HTTPNetwork::GetVersion() const noexcept
{
    return {1, 0, 0};
}

ziapi::Version zia::modules::network::HTTPNetwork::GetCompatibleApiVersion() const noexcept
{
    return {4, 0, 0};
}

const char *zia::modules::network::HTTPNetwork::GetName() const noexcept
{
    return "HTTP network module";
}

const char *zia::modules::network::HTTPNetwork::GetDescription() const noexcept
{
    return "je suis un gentil petit module de merde";
}

void zia::modules::network::HTTPNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests,
    ziapi::http::IResponseInputQueue &responses
)
{
    if (!_init) {
        Debug::err("HTTP Network not init");
        Terminate();
        return;
    }
    startAccept(requests);
    _io_context.post(
        std::bind(&HTTPNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
    _io_context.post(std::bind(&HTTPNetwork::disconnectClient, this));
    if (_thread.joinable()) {
        _thread.join();
    }
    _io_context.restart();
    _thread = std::thread([&]() {
        while (!_io_context.stopped()) {
            _io_context.run_one();
        }
    });
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);
    Debug::log("HTTP network module started");
}

void zia::modules::network::HTTPNetwork::Terminate()
{
    _io_context.stop();
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    Debug::log("HTTP network module terminated");
}

void zia::modules::network::HTTPNetwork::startAccept(
    ziapi::http::IRequestOutputQueue &requests
)
{
    auto newClient = std::make_unique<zia::modules::network::HTTPClient>(
        HTTPClient(_io_context));

    _acceptor.async_accept(newClient->getAsioSocket(),
        std::bind(&HTTPNetwork::handleAccept, this, std::ref(requests),
            std::ref(*newClient)));
    _clients.push_back(std::move(newClient));
}

void zia::modules::network::HTTPNetwork::handleAccept(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPClient &client
)
{
    Debug::log("HTTP Client connected");
    startReceive(requests, client);
    startAccept(requests);
}

void zia::modules::network::HTTPNetwork::startReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPClient &client
)
{
    std::string delimiter = "\r\n\r\n";
    asio::async_read_until(client.getAsioSocket(),
        asio::dynamic_buffer(client.getBuffer()), delimiter,
        std::bind(&HTTPNetwork::handleReceive, this, std::ref(requests),
            std::ref(client), std::placeholders::_1, std::placeholders::_2));
}

void zia::modules::network::HTTPNetwork::handleReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPClient &client, const std::error_code &error,
    std::size_t bytes_transfered
)
{
    client.setProcessingARequest(true);
    if (error == asio::error::eof) {
        client.setConnectionStatut(false);
        return;
    }
    if (!client.isConnected()) {
        return;
    }
    client.saveBuffer();
    client.clearBuffer();
    try {
        auto req = zia::modules::network::HTTPParser::createRequest(
            client.toString());
        if (zia::modules::network::HTTPParser::isRequestComplete(req)) {
            client.setProcessingARequest(true);
            requests.Push(std::make_pair(req, client.getContext()));
            client.clearRawRequest();
            client.setKeepAlive(req);
        } else {
            startReceive(requests, client);
        }
    } catch (const std::invalid_argument &error) {
        Debug::warn(error.what());
    } catch (const std::out_of_range &error) {
        Debug::warn(error.what());
    }
}

void zia::modules::network::HTTPNetwork::sendResponses(
    ziapi::http::IResponseInputQueue &responses,
    ziapi::http::IRequestOutputQueue &requests
)
{
    if (responses.Size() > 0) {
        auto current = responses.Pop();
        if (current.has_value()) {
            auto response = current.value().first;
            auto ctx = current.value().second;
            auto client = std::find_if(_clients.begin(), _clients.end(),
                [&ctx](const std::unique_ptr<HTTPClient> &c) {
                    return *c == ctx;
                });
            if (client != _clients.cend()) {
                auto data = zia::modules::network::HTTPParser::readResponse(
                    response);
                genericSend(**client, &*data.begin(),
                    data.size() * sizeof(*data.begin()), responses, requests);
            }
        }
    }
    _io_context.post(
        std::bind(&HTTPNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
}

void zia::modules::network::HTTPNetwork::disconnectClient() noexcept
{
    if (!_clients.empty()) {
        auto toDelete = std::find_if(_clients.begin(), _clients.end(),
            [](const std::unique_ptr<HTTPClient> &client) {
                if (client->isProcessingARequest()) {
                    return false;
                }
                if (!client->isConnected()) {
                    return true;
                }
                auto keepAlive = client->getKeepAliveInfos();
                if (!keepAlive.has_value()) {
                    return true;
                }
                if (keepAlive.value().max == 0) {
                    return true;
                }
                auto time = std::chrono::steady_clock::now();
                const auto &clientTime = client->getTimeLastRequest();
                if (std::chrono::duration_cast<std::chrono::seconds>(
                    time - clientTime) >=
                    std::chrono::seconds(keepAlive.value().timeout)) {
                    return true;
                }
                return false;
            });
        if (toDelete != _clients.cend()) {
            _clients.erase(toDelete);
        }
    }
    _io_context.post(std::bind(&HTTPNetwork::disconnectClient, this));
}

void zia::modules::network::HTTPNetwork::genericSend(
    zia::modules::network::HTTPClient &client, const void *data,
    const std::size_t &size, ziapi::http::IResponseInputQueue &responses,
    ziapi::http::IRequestOutputQueue &requests
)
{
//   Debug::log("Want to send == " + std::string(static_cast<const char *>(data)));
    client.getAsioSocket().async_send(asio::buffer(data, size),
        [&, this](const asio::error_code &errorCode,
            std::size_t bytesTransferred
        ) {
            if (errorCode) {
                client.setConnectionStatut(false);
            } else if (!client.getKeepAliveInfos().has_value()) {
                client.setConnectionStatut(false);
                client.setProcessingARequest(false);
            } else {
                this->startReceive(requests, client);
            }
            client.updateTime();
            Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
        });
}

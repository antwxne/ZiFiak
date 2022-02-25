/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include <thread>

#include "Modules/Http/HttpModule.hpp"
#include "BasicNetwork.hpp"
#include "Debug/Debug.hpp"

zia::modules::network::BasicNetwork::BasicNetwork()
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->Terminate();
    });
}

zia::modules::network::BasicNetwork::~BasicNetwork()
{
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    _io_context.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
    Debug::log("HTTP network module Destroyed");
}

void zia::modules::network::BasicNetwork::Init(const ziapi::config::Node &cfg)
{
    Debug::log("Init HTTP network module");

    int port = cfg["http"]["port"].AsInt();
    asio::ip::tcp::endpoint basicEndPoint(
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    _acceptor.open(basicEndPoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(basicEndPoint);
    _acceptor.listen();
}

ziapi::Version zia::modules::network::BasicNetwork::GetVersion() const noexcept
{
    return {2, 0, 0};
}

ziapi::Version zia::modules::network::BasicNetwork::GetCompatibleApiVersion() const noexcept
{
    return {4, 0, 0};
}

const char *zia::modules::network::BasicNetwork::GetName() const noexcept
{
    return "HTTP network module";
}

const char *zia::modules::network::BasicNetwork::GetDescription() const noexcept
{
    return "je suis un gentil petit module de merde";
}

void zia::modules::network::BasicNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests,
    ziapi::http::IResponseInputQueue &responses
)
{
    startAccept(requests);
    _io_context.post(
        std::bind(&BasicNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
    _io_context.post(std::bind(&BasicNetwork::disconnectClient, this));
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

void zia::modules::network::BasicNetwork::Terminate()
{
    _io_context.stop();
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    Debug::log("HTTP network module stopped");
}

void zia::modules::network::BasicNetwork::startAccept(
    ziapi::http::IRequestOutputQueue &requests
)
{
    auto newClient = std::make_unique<zia::modules::network::Client>(
        Client(_io_context));

    _acceptor.async_accept(newClient->getAsioSocket(),
        std::bind(&BasicNetwork::handleAccept, this, std::ref(requests),
            std::ref(*newClient)));
    _clients.push_back(std::move(newClient));
}

void zia::modules::network::BasicNetwork::handleAccept(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::Client &client
)
{
    Debug::log("HTTP Client connected");
    startReceive(requests, client);
    startAccept(requests);
}

void zia::modules::network::BasicNetwork::startReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::Client &client
)
{
    std::string delimiter = "\r\n\r\n";
    asio::async_read_until(client.getAsioSocket(),
        asio::dynamic_buffer(client.getBuffer()), delimiter,
        std::bind(&BasicNetwork::handleReceive, this, std::ref(requests),
            std::ref(client), std::placeholders::_1, std::placeholders::_2));
}

void zia::modules::network::BasicNetwork::handleReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::Client &client, const std::error_code &error,
    std::size_t bytes_transfered
)
{
    client.setProcessingARequest(true);
    if (error == asio::error::eof) {
        Debug::log("HTTP Client disconnected");
        client.setConnectionStatut(false);
        return;
    }
    if (!client.isConnected()) {
        return;
    }
    client.saveBuffer();
    client.clearBuffer();
    try {
        auto req = zia::modules::http::HttpModule::createRequest(
            client.toString());
        //        ziapi::http::Request req;
        //        req.headers = {{ziapi::http::header::kAIM, "aada"}};
        //        req.method = ziapi::http::method::kGet;
        //        req.version = ziapi::http::Version::kV1;
        //        req.target = "/";
        //        req.body = "toto";
        if (zia::modules::http::HttpModule::isRequestComplete(req)) {
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

void zia::modules::network::BasicNetwork::sendResponses(
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
                [&ctx](const std::unique_ptr<Client> &c) {
                    return *c == ctx;
                });
            if (client != _clients.cend()) {
                auto data = zia::modules::http::HttpModule::readResponse(
                    response);
                genericSend(**client, &*data.begin(),
                    data.size() * sizeof(*data.begin()), responses, requests);
            }
        }
    }
    _io_context.post(
        std::bind(&BasicNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
}

void zia::modules::network::BasicNetwork::disconnectClient() noexcept
{
    if (!_clients.empty()) {
        auto toDelete = std::find_if(_clients.begin(), _clients.end(),
            [](const std::unique_ptr<Client> &client) {
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
    _io_context.post(std::bind(&BasicNetwork::disconnectClient, this));
}

void zia::modules::network::BasicNetwork::genericSend(
    zia::modules::network::Client &client, const void *data,
    const std::size_t &size, ziapi::http::IResponseInputQueue &responses,
    ziapi::http::IRequestOutputQueue &requests
)
{
    try {
        client.getAsioSocket().async_send(asio::buffer(data, size),
            [&, this](const asio::error_code &errorCode,
                std::size_t bytesTransferred
            ) {
                if (errorCode) {
                    throw MyException(errorCode.message(), __PRETTY_FUNCTION__,
                        __FILE__, __LINE__);
                }
                if (!client.getKeepAliveInfos().has_value()) {
                    client.setConnectionStatut(false);
                    client.setProcessingARequest(false);
                } else {
                    this->startReceive(requests, client);
                }
                client.updateTime();
                Debug::log(
                    std::to_string(bytesTransferred) + " bytes transferred");
            });
    }  catch (const MyException &e) {
        client.setConnectionStatut(false);
        Debug::log(e);
    }
}



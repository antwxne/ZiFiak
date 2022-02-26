/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include <thread>

#include "ziapi/Http.hpp"
#include "HTTPSNetwork.hpp"
#include "Modules/Network/HTTPParser/HTTPParser.hpp"

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new zia::modules::network::HTTPSNetwork;
}
zia::modules::network::HTTPSNetwork::HTTPSNetwork()
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context),
    _sslContext(asio::ssl::context::sslv23)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->Terminate();
    });
}

zia::modules::network::HTTPSNetwork::~HTTPSNetwork()
{
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    _io_context.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
    Debug::log("HTTPS network module Destroyed");
}

void zia::modules::network::HTTPSNetwork::Init(const ziapi::config::Node &cfg)
{
    Debug::log("Init HTTPS network module");
    std::string certificate_path = cfg["https"]["certificate_path"].AsString();
    std::string private_key_file = cfg["https"]["private_key_file"].AsString();
    int port = cfg["https"]["port"].AsInt();

    _sslContext.set_options(
        asio::ssl::context::default_workarounds | asio::ssl::context::no_sslv2 |
            asio::ssl::context::single_dh_use);
    _sslContext.use_certificate_chain_file(certificate_path);
    _sslContext.use_private_key_file(private_key_file, asio::ssl::context::pem);
    asio::ip::tcp::endpoint basicEndPoint(
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    _acceptor.open(basicEndPoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(basicEndPoint);
    _acceptor.listen();
}

ziapi::Version zia::modules::network::HTTPSNetwork::GetVersion() const noexcept
{
    return {1, 0, 0};
}

ziapi::Version zia::modules::network::HTTPSNetwork::GetCompatibleApiVersion() const noexcept
{
    return {4, 0, 0};
}

const char *zia::modules::network::HTTPSNetwork::GetName() const noexcept
{
    return "HTTPS network module";
}

const char *zia::modules::network::HTTPSNetwork::GetDescription() const noexcept
{
    return "Only fan in bio";
}

void zia::modules::network::HTTPSNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests,
    ziapi::http::IResponseInputQueue &responses
)
{
    startAccept(requests);
    _io_context.post(
        std::bind(&HTTPSNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
    _io_context.post(std::bind(&HTTPSNetwork::disconnectClient, this));
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

void zia::modules::network::HTTPSNetwork::Terminate()
{
    _io_context.stop();
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    Debug::log("HTTP network module stopped");
}

void zia::modules::network::HTTPSNetwork::startAccept(
    ziapi::http::IRequestOutputQueue &requests
)
{
    auto newClient = std::make_unique<zia::modules::network::HTTPSClient>(
        HTTPSClient(_io_context, _sslContext));

    _acceptor.async_accept(newClient->getAsioSocket(),
        std::bind(&HTTPSNetwork::handleAccept, this, std::ref(requests),
            std::ref(*newClient)));
    _clients.push_back(std::move(newClient));
}

void zia::modules::network::HTTPSNetwork::handleAccept(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPSClient &client
)
{
    Debug::log("HTTPS Client connected");
    client.initSSL();
    client.getAsioSSLSocket()->async_handshake(asio::ssl::stream_base::server,
        [&](const std::error_code &error) {
            if (!error) {
                startReceive(requests, client);
            }
            if (error) {
                client.setConnectionStatut(false);
            }
        });
    startAccept(requests);
}

void zia::modules::network::HTTPSNetwork::startReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPSClient &client
)
{
    std::string delimiter = "\r\n\r\n";
    asio::async_read_until(*client.getAsioSSLSocket(),
        asio::dynamic_buffer(client.getRawRequest()), delimiter,
        std::bind(&HTTPSNetwork::handleReceive, this, std::ref(requests),
            std::ref(client), std::placeholders::_1, std::placeholders::_2));
}

void zia::modules::network::HTTPSNetwork::handleReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::HTTPSClient &client, const std::error_code &error,
    std::size_t bytes_transfered
)
{
    client.setProcessingARequest(true);
    if (error == asio::error::eof) {
        Debug::log("HTTPS Client disconnected");
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
        //        ziapi::http::Request req;
        //        req.headers = {{ziapi::http::header::kAIM, "aada"}};
        //        req.method = ziapi::http::method::kGet;
        //        req.version = ziapi::http::Version::kV1;
        //        req.target = "/";
        //        req.body = "toto";
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

void zia::modules::network::HTTPSNetwork::sendResponses(
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
                [&ctx](const std::unique_ptr<HTTPSClient> &c) {
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
        std::bind(&HTTPSNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
}

void zia::modules::network::HTTPSNetwork::disconnectClient() noexcept
{
    if (!_clients.empty()) {
        auto toDelete = std::find_if(_clients.begin(), _clients.end(),
            [](const std::unique_ptr<HTTPSClient> &client) {
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
    _io_context.post(std::bind(&HTTPSNetwork::disconnectClient, this));
}

void zia::modules::network::HTTPSNetwork::genericSend(
    zia::modules::network::HTTPSClient &client, const void *data,
    const size_t &size, ziapi::http::IResponseInputQueue &responses,
    ziapi::http::IRequestOutputQueue &requests
)
{

    asio::async_write(*client.getAsioSSLSocket(), asio::buffer(data, size),
        asio::bind_executor(client.getStrand(),
            [&, this](const asio::error_code &errorCode,
                std::size_t bytesTransferred
            ) {
                if (errorCode) {
                    client.setConnectionStatut(false);
                }
                else if (!client.getKeepAliveInfos().has_value()) {
                    client.setConnectionStatut(false);
                    client.setProcessingARequest(false);
                } else {
                    this->startReceive(requests, client);
                }
                client.updateTime();
                Debug::log(
                    std::to_string(bytesTransferred) + " bytes transferred");
            }));
}

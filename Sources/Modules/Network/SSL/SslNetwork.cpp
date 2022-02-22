/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include <thread>

#include "Modules/Http/HttpModule.hpp"
#include "SslNetwork.hpp"

zia::modules::network::SSLNetwork::SSLNetwork()
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context),
    _isRunning(false), _sslContext(asio::ssl::context::sslv23)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->Terminate();
    });
}

void zia::modules::network::SSLNetwork::Init(const ziapi::config::Node &cfg)
{
    Debug::log("init server");
    std::string permFilePath = cfg["https"]["certificat_file_path"].AsString();
    int port = cfg["https"]["port"].AsInt();

    _sslContext.set_options(
        asio::ssl::context::default_workarounds | asio::ssl::context::no_sslv2 |
            asio::ssl::context::single_dh_use);
    _sslContext.use_certificate_chain_file(permFilePath);
    _sslContext.use_private_key_file(permFilePath, asio::ssl::context::pem);
    asio::ip::tcp::endpoint basicEndPoint(
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    _acceptor.open(basicEndPoint.protocol());
    _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(basicEndPoint);
    _acceptor.listen();
}

ziapi::Version zia::modules::network::SSLNetwork::GetVersion() const noexcept
{
    return {1, 0, 0};
}

ziapi::Version zia::modules::network::SSLNetwork::GetCompatibleApiVersion() const noexcept
{
    return {3, 0, 0};
}

const char *zia::modules::network::SSLNetwork::GetName() const noexcept
{
    return "SLL network module";
}

const char *zia::modules::network::SSLNetwork::GetDescription() const noexcept
{
    return "SSL module for https connection";
}

void zia::modules::network::SSLNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests,
    ziapi::http::IResponseInputQueue &responses
)
{
    Debug::log("Server Start");
    _isRunning = true;
    startAccept(requests);
    _io_context.run();
    _responseThread = std::thread(&SSLNetwork::sendResponses, this,
        std::ref(responses));
    _disconnectClientThread = std::thread(&SSLNetwork::disconnectClient, this);
}

void zia::modules::network::SSLNetwork::Terminate()
{
    Debug::log("server stop");
    _isRunning = false;
    _io_context.stop();
    _responseThread.join();
    _disconnectClientThread.join();
}

void zia::modules::network::SSLNetwork::startAccept(
    ziapi::http::IRequestOutputQueue &requests
)
{
    _clients.emplace_back(std::make_unique<zia::modules::network::SSLClient>(
        SSLClient(_io_context, _sslContext)));
    _acceptor.async_accept(_clients.back()->getAsioSocket(),
        std::bind(&SSLNetwork::handleAccept, this, std::ref(requests),
            std::ref(*_clients.back().get())));
}

void zia::modules::network::SSLNetwork::handleAccept(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::SSLClient &client
)
{
    Debug::log("SSLClient connected");
    client.initSSL();
    startReceive(requests, client);
    startAccept(requests);
}

void zia::modules::network::SSLNetwork::startReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::SSLClient &client
)
{
    std::string delimiter = "\r\n\r\n";
    asio::async_read_until(*client.getAsioSSLSocket(),
        asio::dynamic_buffer(client.getRawRequest()), delimiter,
        std::bind(&SSLNetwork::handleReceive, this, std::ref(requests),
            std::ref(client), std::placeholders::_1, std::placeholders::_2));
}

void zia::modules::network::SSLNetwork::handleReceive(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::SSLClient &client, const std::error_code &error,
    std::size_t bytes_transfered
)
{
    if (error == asio::error::eof) {
        Debug::log("SSLClient disconected");
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
        if (zia::modules::http::HttpModule::isRequestComplete(req)) {
            client.setProcessingARequest(true);
            requests.Push(std::make_pair(req, client.getContext()));
            client.clearRawRequest();
        }
        client.setKeepAlive(req);
    } catch (const std::invalid_argument &error) {
        Debug::warn(error.what());
    } catch (const std::out_of_range &error) {
        Debug::warn(error.what());
    }
    startReceive(requests, client);
}

void zia::modules::network::SSLNetwork::sendResponses(
    ziapi::http::IResponseInputQueue &responses
)
{
    while (_isRunning) {
        while (responses.Size() > 0) {
            auto current = responses.Pop();
            if (!current.has_value()) {
                continue;
            }
            auto response = current.value().first;
            auto ctx = current.value().second;
            auto client = std::find_if(_clients.begin(), _clients.end(),
                [&ctx](const std::unique_ptr<SSLClient> &c) {
                    return *c == ctx;
                });
            if (client != _clients.cend()) {
                *client->get() << response;
            }
        }
    }
}

void zia::modules::network::SSLNetwork::disconnectClient() noexcept
{
    while (_isRunning) {
        _clients.erase(std::find_if(_clients.begin(), _clients.end(),
            [](const std::unique_ptr<SSLClient> &client) {
                auto keepAlive = client->getKeepAliveInfos();
                if (!client->isConnected()) {
                    return true;
                }
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
            }), _clients.end());
    }
}

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
    : _io_context(), _acceptor(_io_context), _signalSet(_io_context), _sslContext(asio::ssl::context::sslv23)
{
    _signalSet.add(SIGINT);
    _signalSet.add(SIGTERM);

    _signalSet.async_wait([this](std::error_code ec, int signo) {
        this->Terminate();
    });
}

zia::modules::network::SSLNetwork::~SSLNetwork()
{
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    _io_context.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
    Debug::log("HTTPS network module Destroyed");
}

void zia::modules::network::SSLNetwork::Init(const ziapi::config::Node &cfg)
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
    return "HTTPS network module";
}

const char *zia::modules::network::SSLNetwork::GetDescription() const noexcept
{
    return "Only fan in bio";
}

void zia::modules::network::SSLNetwork::Run(
    ziapi::http::IRequestOutputQueue &requests,
    ziapi::http::IResponseInputQueue &responses
)
{
    startAccept(requests);
    _io_context.post(std::bind(&SSLNetwork::sendResponses, this,
        std::ref(responses), std::ref(requests)));
    _io_context.post(std::bind(&SSLNetwork::disconnectClient, this));
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

void zia::modules::network::SSLNetwork::Terminate()
{
    _io_context.stop();
    _signalSet.remove(SIGINT);
    _signalSet.remove(SIGTERM);
    Debug::log("HTTP network module stopped");

}

void zia::modules::network::SSLNetwork::startAccept(
    ziapi::http::IRequestOutputQueue &requests
)
{
    auto newClient = std::make_unique<zia::modules::network::SSLClient>(
        SSLClient(_io_context, _sslContext));

    _acceptor.async_accept(newClient->getAsioSocket(),
        std::bind(&SSLNetwork::handleAccept, this, std::ref(requests),
            std::ref(*newClient)));
    _clients.push_back(std::move(newClient));
}

void zia::modules::network::SSLNetwork::handleAccept(
    ziapi::http::IRequestOutputQueue &requests,
    zia::modules::network::SSLClient &client
)
{
    Debug::log("HTTPS Client connected");
    try {
        client.initSSL();
        startReceive(requests, client);
    } catch (const std::runtime_error &error) {
        Debug::log(error.what());
    }
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

void zia::modules::network::SSLNetwork::sendResponses(
    ziapi::http::IResponseInputQueue &responses, ziapi::http::IRequestOutputQueue &requests
)
{
    if (responses.Size() > 0) {
        auto current = responses.Pop();
        if (current.has_value()) {
            auto response = current.value().first;
            auto ctx = current.value().second;
            auto client = std::find_if(_clients.begin(), _clients.end(),
                [&ctx](const std::unique_ptr<SSLClient> &c) {
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
        std::bind(&SSLNetwork::sendResponses, this, std::ref(responses),
            std::ref(requests)));
}

void zia::modules::network::SSLNetwork::disconnectClient() noexcept
{
    if (!_clients.empty()) {
        auto toDelete = std::find_if(_clients.begin(), _clients.end(),
            [](const std::unique_ptr<SSLClient> &client) {
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
    _io_context.post(std::bind(&SSLNetwork::disconnectClient, this));
}

void zia::modules::network::SSLNetwork::genericSend(
    zia::modules::network::SSLClient &client, const void *data,
    const size_t &size, ziapi::http::IResponseInputQueue &responses,
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
    } catch (const MyException &e) {
        client.setConnectionStatut(false);
        Debug::log(e);
    }
}

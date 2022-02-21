//
// Created by Clement Ruat on 18/02/2022.
//

#include "SSLCertificate.hpp"
#include <asio.hpp>
#include <iostream>
#include <asio/ssl.hpp>

void SSLCertificate::verifyCertificateClient() {
    asio::ssl::context context(asio::ssl::context::sslv23);
    context.set_default_verify_paths();

    asio::io_context ioContext;
    asio::ssl::stream<asio::ip::tcp::socket> sock(ioContext, context);
    asio::ip::tcp::resolver resolver(ioContext);
    asio::ip::tcp::resolver::query query("host.name", "https");
    asio::connect(sock.lowest_layer(), resolver.resolve(query));
    sock.lowest_layer().set_option(asio::ip::tcp::no_delay(true));

    try {
        sock.set_verify_mode(asio::ssl::verify_peer);
        sock.set_verify_callback(asio::ssl::rfc2818_verification("host.name"));
        sock.handshake(asio::ssl::stream<asio::ip::tcp::socket>::client);
    } catch(std::exception &e) {
        std::cerr << "error wrong certificate :" << e.what() << std::endl;
    }
   // ssl_socket asio::ssl:(ioContext, context);
}

void SSLCertificate::verifyCertificateServer() {

}
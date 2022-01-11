/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#ifndef BASIC_SERVER_CLIENT_HPP
#define BASIC_SERVER_CLIENT_HPP

#include <asio.hpp>
#include <vector>
#include <iostream>

#include "Exceptions/MyException.hpp"
#include "Debug/Debug.hpp"

namespace Server_n {
class Client {
public:
    Client();

    template<typename T>
    Client &operator<<(const T &obj)
    {
        this->_socket.async_send(asio::buffer(obj, sizeof(T),
            [](const asio::error_code &errorCode, std::size_t bytesTransferred) {
                if (errorCode) {
                    throw MyException(errorCode.message(), __PRETTY_FUNCTION__, __FILE__, __LINE__);
                }
                Debug::log(std::to_string(bytesTransferred) + " bytes transferred");
            }));
        return *this;
    }

private:
    asio::ip::tcp::socket _socket;
    std::vector<uint8_t> _buffer;
};


}

#endif //BASIC_SERVER_CLIENT_HPP

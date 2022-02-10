/*
** EPITECH PROJECT, 2024
** Basic_server
** File description:
** Created by antoine,
*/

#include <exception>
#include "Debug/Debug.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"
#include <iostream>

zia::server::Server::Server()
{

}

void zia::server::Server::init(const std::string &filepath)
{
    Debug::log("init server");

    ConfigParser confParser;

    try {
        this->_serverConfig = ConfigParser::loadFromFile(filepath);
        Debug::log("config load successfully loaded");
    } catch(const std::runtime_error& e) {
        Debug::warn("failed to load config file: " + std::string(e.what()));
        this->_serverConfig = Node(ziapi::config::Undefined{});
    }
}

void zia::server::Server::run()
{
    Debug::log("server running");

}



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

zia::server::Server::Server() {

}

void zia::server::Server::init(const std::string &filepath) {
    Debug::log("init server");

    ConfigParser confParser;

    try {
        this->_serverConfig = ConfigParser::loadFromFile(filepath);
        Debug::log("config load successfully loaded");
    } catch (const std::runtime_error &e) {
        Debug::warn("failed to load config file: " + std::string(e.what()));
        this->_serverConfig = Node(ziapi::config::Undefined{});
    }
}

const std::string &zia::server::Server::getPathDirectory() const {
    try {
        return _serverConfig["directory"].AsString();
    } catch (std::runtime_error &e) {
        std::cerr << "Error module Directory" << std::endl;
        return "./modules";
    }
}

void zia::server::Server::run() {
    auto pathDirectory = zia::server::Server::getPathDirectory();
    Debug::log("server running");
}

const ziapi::config::Node &zia::server::Server::getServerConfig() const {
    return _serverConfig;
}
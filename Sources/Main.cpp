/*
** EPITECH PROJECT, 2024
** Project
** File description:
** Created by antoine,
*/

#include <iostream>
#include <cstdlib>
#include <regex>
#include <fstream>

#include "Debug/Debug.hpp"
#include "Server/Server.hpp"

int main(int ac, const char **av)
{
    zia::server::Server server;
    nlohmann::json config;

    if (ac != 2) {
        std::cout << "please provide a port" << std::endl;
        return 84;
    }
    std::ifstream input(av[1]);
    input >> config;
    try {
        server.init(config);
        server.run();
    } catch (const MyException &e) {
        Debug::err(e);
        return 84;
    } catch (const std::exception &e) {
        std::regex space("[[:space:]]");
        std::string search = "https://www.google.com/search?q=" + std::string(e.what());
        Debug::err(std::regex_replace(search, space, "%20"));
        return 84;
    }
    return 0;
}
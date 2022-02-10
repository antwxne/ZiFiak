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
#include "dylib/dylib.hpp"
#include "LoadLib/LoadLib.hpp"

int main(int ac, char **av)
{
    // if (ac != 2) {
    //     std::cout << "please provide a port" << std::endl;
    //     return 84;
    // }
    // std::ifstream input(av[1]);
    // input >> config;
    zia::server::Server server;
    try {
        server.init("./Config/server_cfg.json");
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
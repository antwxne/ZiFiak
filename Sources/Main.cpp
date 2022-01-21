/*
** EPITECH PROJECT, 2024
** Project
** File description:
** Created by antoine,
*/

#include <iostream>
#include <cstdlib>
#include <regex>

#include "Debug/Debug.hpp"
#include "Server/Server.hpp"

#include "AModule.hpp"

void plop()
{
    using namespace std::string_literals;

    JObject plouf = {
        JField("firstName"s, "phn"s), JField{"lastName"s, "Smith"s},
        JField{"isAlive"s, true}, JField{"age"s, 27},


    }


}




int main(int ac, char **av)
{
    if (ac != 2) {
        std::cout << "please provide a port" << std::endl;
        return 84;
    }
    try {
        Server_n::Server server(std::atoi(av[1]));
        std::cout << "hello world!" << std::endl;
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
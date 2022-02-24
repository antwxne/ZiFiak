/*
** EPITECH PROJECT, 2024
** Project
** File description:
** Created by antoine,
*/

#include <iostream>
#include <regex>
#include <fstream>

#include "Debug/Debug.hpp"
#include "Server/Server.hpp"
#include "nlohmann/json.hpp"

#include "Server/ConfigParser.hpp"
#include "Modules/Network/Basic/BasicNetwork.hpp"
#include "Queue/ResponseQueue.hpp"
#include "Queue/RequestQueue.hpp"


int main(int ac, char **av)
{
    if (ac != 2) {
         std::cout << "please provide a port" << std::endl;
         return 84;
    }

    std::string filepath(av[1]);
    auto cfg = zia::ConfigParser::loadFromFile(filepath);
//    zia::server::Server server;
    zia::modules::network::BasicNetwork network;
    try {
//        server.init(av[1]);
//        server.run();
        zia::container::ResponseQueue responses;
        zia::container::RequestQueue requests;
        ziapi::http::Response res;
        network.Init(cfg);
        network.Run(requests, responses);
        Debug::log("apres le run");
//        network.Terminate();
        while(1) {
            if (requests.size() > 0) {
                auto plop = requests.Pop();
                Debug::log("request poped");
                res.Bootstrap();
                responses.push_back(std::make_pair(res, plop->second));
                Debug::log("response pushed");
            }
        }
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
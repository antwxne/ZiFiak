/*
** EPITECH PROJECT, 2024
** Project
** File description:
** Created by antoine,
*/

#include <iostream>
#include <regex>

#include "Debug/Debug.hpp"
#include "Server/Server.hpp"

int main(int ac, char **av)
{
    if (ac > 2) {
        std::cout << "please provide one config file path or nothing for basic config" << std::endl;
        return 84;
    }
    zia::server::Server server;
    try {
        server.init(ac != 2 ? "" : av[1]);
        server.run();
    } catch (const MyException &e) {
        Debug::err(e);
        return 84;
    } catch (const std::exception &e) {
        std::regex space("[[:space:]]");
        std::string search =
            "https://www.google.com/search?q=" + std::string(e.what());
        Debug::err(std::regex_replace(search, space, "%20"));
        return 84;
    }
    return 0;
}


// pour tester le network si vous voulez
//

//#include "nlohmann/json.hpp"
//#include <fstream>
//#include "Server/ConfigParser.hpp"
//#include "Modules/Network/HTTP/HTTPNetwork.hpp"
//#include "Queue/ResponseQueue.hpp"
//#include "Queue/RequestQueue.hpp"
//
//std::string filepath(av[1]);
//auto cfg = zia::ConfigParser::loadFromFile(filepath);
//zia::modules::network::HTTPNetwork network;
//try {
//
//zia::container::ResponseQueue responses;
//zia::container::RequestQueue requests;
//ziapi::http::Response res;
//network.Init(cfg);
//network.Run(requests, responses);
//while(true) {
//if (!requests.empty()) {
//auto plop = requests.Pop();
//Debug::log("request poped");
//res.Bootstrap();
//res.body = "skdjhqkfjlqjfkhdsqlfkjhskhkjsqhfksqjf";
//responses.push_back(std::make_pair(res, plop->second));
//Debug::log("response pushed");
//}
//}
//} catch (const MyException &e) {
//Debug::err(e);
//return 84;
//} catch (const std::exception &e) {
//std::regex space("[[:space:]]");
//std::string search = "https://www.google.com/search?q=" + std::string(e.what());
//Debug::err(std::regex_replace(search, space, "%20"));
//return 84;
//}

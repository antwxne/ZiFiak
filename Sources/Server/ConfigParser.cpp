/*
** EPITECH PROJECT, 2022
** test
** File description:
** ConfigParser
*/

#include <map>
#include <memory>
#include <fstream>
#include <exception>
#include "ConfigParser.hpp"

namespace zia {

ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

ConfigParser::Node ConfigParser::createArray(const JsonParser &json)
{
    std::vector<std::shared_ptr<Node>> vecNode;

    for (auto &elem : json) {
        vecNode.push_back(std::make_shared<Node>(ConfigParser::convertJsonToNode(elem)));
    }
    return Node({vecNode});
}

ConfigParser::Node ConfigParser::createObj(const JsonParser &json)
{
    std::unordered_map<std::string, std::shared_ptr<Node>> vecNode;

    for (auto &elem : json.items()) {
        vecNode.insert({elem.key(), std::make_shared<Node>(ConfigParser::convertJsonToNode(elem.value()))});
    }
    return Node({vecNode});
}

ConfigParser::Node ConfigParser::convertJsonToNode(const JsonParser &json)
{
    auto type = json.type();

    switch (type) {
    case JsonParser::value_t::null:
        return Node(nullptr);
    case JsonParser::value_t::boolean:
        return Node(json.get<bool>());
    case JsonParser::value_t::number_unsigned:
        return Node(json.get<int>());
    case JsonParser::value_t::number_integer:
        return Node(json.get<int>());
    case JsonParser::value_t::number_float:
        return Node(json.get<double>());
    case JsonParser::value_t::string:
        return Node(json.get<std::string>());
    case JsonParser::value_t::array:
        return ConfigParser::createArray(json);
    case JsonParser::value_t::object:
        return ConfigParser::createObj(json);
    default:
        return Node(ziapi::config::Undefined{});
    }
}

ConfigParser::Node ConfigParser::loadFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    JsonParser json;

    if (!file.is_open())
        throw std::system_error(errno, std::system_category(), "Failed to open " + filePath);
    try {
        file >> json;
    } catch(const std::exception& e) {
        throw std::runtime_error("Invalid json file: " + std::string(e.what()));
    }
    return ConfigParser::convertJsonToNode(json);
}

}
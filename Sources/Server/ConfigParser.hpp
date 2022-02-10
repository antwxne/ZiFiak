/*
** EPITECH PROJECT, 2022
** test
** File description:
** ConfigParser
*/

#ifndef CONFIGPARSER_HPP_
#define CONFIGPARSER_HPP_

#include <string>
#include "nlohmann/json.hpp"
#include "ziapi/Config.hpp"

namespace zia {

class ConfigParser {
    public:
        using JsonParser = nlohmann::json;
        using Node = ziapi::config::Node;

        ConfigParser();
        ~ConfigParser();

        Node loadFromFile(const std::string &filePath);

    protected:
    private:
        Node convertJsonToNode(const JsonParser &json) const;
        Node createObj(const JsonParser &json) const;
        Node createArray(const JsonParser &json) const;

        JsonParser _json;
};

}

#endif /* !CONFIGPARSER_HPP_ */
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

        static Node loadFromFile(const std::string &filePath);

    protected:
    private:
        static Node convertJsonToNode(const JsonParser &json);
        static Node createObj(const JsonParser &json);
        static Node createArray(const JsonParser &json);
};

}

#endif /* !CONFIGPARSER_HPP_ */
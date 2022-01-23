/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_JSON_HPP
#define ZIA_JSON_HPP

#include <variant>
#include <unordered_map>
#include <string>
#include <any>
#include <vector>

using JSONKey = std::string;
using JSONValue = std::variant<std::nullptr_t, int, std::string, bool, std::unordered_map<JSONKey, std::any>>;
using JSONObject = std::unordered_map<JSONKey, JSONValue>;

#endif //ZIA_JSON_HPP

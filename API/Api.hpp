/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_API_HPP
#define ZIA_API_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "Module/IModule.hpp"

namespace API_n {

/**
 * @brief enum for execution phase 
 * 
 */
enum phase_e : char {
    CGI,
    PRE_REQ,
    POST_REQ,
    PRE_PARSE,
    POST_PARSE
};

/**
 * @brief Api class 
 * 
 */
class API {
public:
    /**
     * @brief Construct a new API object
     *
     */
    API() = default;
    /**
     * @brief Destroy the API object
     *
     */
    ~API() = default;
    /**
     * @brief loadModule
     *
     * @param name
     */
    void loadModule(const std::string &name);
    /**
     * @brief handleRequest
     * call all the module in the correct execution order.
     *
     * @param context (contains client information)
     */
    void handleRequest(IContext &context);

private:
    /**
     * @brief unordered_map for modules
     *
     */
    std::unordered_map<phase_e, std::vector<std::shared_ptr<Module_n::IModule>>> _modules;
};
}
#endif //ZIA_API_HPP

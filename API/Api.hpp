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

#include "IModule.hpp"
#include "HTTP/Response/Response.hpp"
#include "HTTP/Request/Request.hpp"

namespace API_n {

/**
 * @brief enum for execution phase 
 * 
 */
    enum phase_e : char {
        PLOP,
        PLOUF
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
         *
         * @param req
         * @return std::shared_ptr<HTTP_n::Response>
         */
        std::shared_ptr <HTTP_n::Response> handleRequest(HTTP_n::Request &req);

    private:
        /**
         * @brief unordered_map for modules
         *
         */
        std::unordered_map <phase_e, std::vector<std::shared_ptr < Module_n::IModule>>>
        _modules;
    };
}
#endif //ZIA_API_HPP

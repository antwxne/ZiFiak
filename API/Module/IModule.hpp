/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_IMODULE_HPP
#define ZIA_IMODULE_HPP

#include <string>

#include "Context/IContext.hpp"

namespace API_n {
enum phase_e : char;
}
namespace Module_n {

/**
 * @brief enum for response status
 * 
 */
enum responseCode_e : char {
    OK,
    KO,
    CONTINUE
};

/**
 * @brief Interface for Module
 * 
 */
class IModule {
public:
    /**
     * @brief Get the Execution Phase object
     *
     * @return API_n::phase_e
     */
    virtual API_n::phase_e getExecutionPhase() const

    noexcept = 0;
    /**
     * @brief
     *
     * @param req
     * @param res
     * @return responseCode_e
     */
    virtual responseCode_e handleRequest(IContext &context) = 0;
    /**
     * @brief Destroy the IModule object
     *
     */
    virtual ~IModule() = default;
};
}
#endif //ZIA_IMODULE_HPP

/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include "JSON.hpp"
#include "IModule.hpp"
#include "Server/Config/Configuration.hpp"

namespace Module_n {

/**
* @brief Abstraction for Module
*
*/
class AModule : public IModule {
public:
    /**
     * @brief Construct a new AModule object
     *
     * @param configuration
     */
    AModule(const JSONObject &configuration);
    /**
     * @brief Destroy the AModule object
     *
     */
    ~AModule() = default;
    /**
     * @brief Get the Execution Phase object
     *
     * @return API_n::phase_e
     */
    API_n::phase_e getExecutionPhase() const noexcept final;

protected:
    API_n::phase_e _phase;
};
}

#endif //ZIA_AMODULE_HPP

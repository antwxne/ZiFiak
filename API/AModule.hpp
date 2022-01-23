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
class AModule : public IModule {
public:
    AModule();
    AModule(const JSONObject &configuration);
    ~AModule() = default;
    API_n::phase_e getExecutionPhase() const noexcept final;

protected:
    API_n::phase_e _phase;
};
}

#endif //ZIA_AMODULE_HPP

/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include "IModule.hpp"
#include "Server/Config/Configuration.hpp"

namespace Module_n {
class AModule: IModule {
    AModule(const Server_n::Configuration &configuration);
    ~AModule() = default;

    API_n::phase_e getExecutionPhase() const noexcept final;

private:
    API_n::phase_e _phase;
};
}

#endif //ZIA_AMODULE_HPP

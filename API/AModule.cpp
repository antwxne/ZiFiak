/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#include "AModule.hpp"

Module_n::AModule::AModule(const JObject &configuration)
{
}

API_n::phase_e Module_n::AModule::getExecutionPhase() const noexcept
{
    return _phase;
}

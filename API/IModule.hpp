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

enum responseCode_e : char {
    OK,
    KO,
    CONTINUE
};

class IModule {
public:
    virtual API_n::phase_e getExecutionPhase() const noexcept = 0;
    virtual responseCode_e handleRequest(IContext &context) = 0; // prendre un contexte (client, buffer, socket, config du serveur(optionel))
};
}
#endif //ZIA_IMODULE_HPP

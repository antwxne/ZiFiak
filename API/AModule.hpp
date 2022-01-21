/*
** EPITECH PROJECT, 2024
** Zia
** File description:
** Created by antoine,
*/

#ifndef ZIA_AMODULE_HPP
#define ZIA_AMODULE_HPP

#include <variant>
#include <unordered_map>
#include <string>
#include <vector>


#include "IModule.hpp"
#include "Server/Config/Configuration.hpp"

struct JValue;

using JArray = std::vector<JValue>;
using JObject = std::vector<std::pair<std::string, JValue>>;
using JField = std::pair<std::string, JValue>;

struct JValue {

    std::variant<std::nullptr_t, int, std::string, bool, JArray, JObject> value;

    template <typename Arg, typename = std::enable_if<std::is_same_v<std::decay_t<Arg>, Jvalue>>>
    JValue(Arg &&arg) : value(std::forward<Arg>(arg)) {}
};


namespace Module_n {
class AModule: public IModule {
public:
    AModule();
    AModule(const JObject &configuration); // prendre un data type json like pour pas imposer un type de config pour le server
    ~AModule() = default;

    API_n::phase_e getExecutionPhase() const noexcept final;

protected:
    API_n::phase_e _phase;
};
}

#endif //ZIA_AMODULE_HPP

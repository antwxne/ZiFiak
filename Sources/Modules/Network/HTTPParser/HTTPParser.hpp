/*
** EPITECH PROJECT, 2022
** ZIA
** File description:
** HttpModule
*/

#ifndef HTTPMODULE_HPP_
#define HTTPMODULE_HPP_

#include <vector>
#include <sstream>
#include <iostream>
#include <regex>
#include <algorithm>

#include "Exceptions/MyException.hpp"
#include "ziapi/Http.hpp"

namespace zia::modules::network {

    static const std::map<std::string, ziapi::http::Version> _versions {
        {"HTTP/1.0", ziapi::http::Version::kV1},
        {"HTTP/1.1", ziapi::http::Version::kV1_1},
        {"HTTP/2.0", ziapi::http::Version::kV2},
        {"HTTP/3.0", ziapi::http::Version::kV3},
    };

    static const std::vector<std::string> _methods {
        ziapi::http::method::kGet,
        ziapi::http::method::kPost,
        ziapi::http::method::kPut,
        ziapi::http::method::kDelete,
        ziapi::http::method::kPatch,
        ziapi::http::method::kOptions,
        ziapi::http::method::kHead,
    };

    class HTTPParser {
        public:
            HTTPParser() = default;
            ~HTTPParser() = default;

            static ziapi::http::Request createRequest(const std::string &str);
            static std::string readResponse(const ziapi::http::Response &res) noexcept;
            static bool isRequestComplete(const ziapi::http::Request &req) noexcept;
            static std::pair<int, int>parseKeepAliveInfos(const std::string &value);
    };

    static const std::map<ziapi::http::Code, std::string> _codes {
        {ziapi::http::Code::kContinue, "100"},
        {ziapi::http::Code::kSwitchingProtocols, "101"},
        {ziapi::http::Code::kOK, "200"},
        {ziapi::http::Code::kCreated, "201"},
        {ziapi::http::Code::kAccepted, "202"},
        {ziapi::http::Code::kNonAuthoritativeInformation, "203"},
        {ziapi::http::Code::kNoContent, "204"},
        {ziapi::http::Code::kResetContent, "205"},
        {ziapi::http::Code::kPartialContent, "206"},
        {ziapi::http::Code::kMultipleChoices, "300"},
        {ziapi::http::Code::kMovedPermanently, "301"},
        {ziapi::http::Code::kFound, "302"},
        {ziapi::http::Code::kSeeOther, "303"},
        {ziapi::http::Code::kNotModified, "304"},
        {ziapi::http::Code::kUseProxy, "305"},
        {ziapi::http::Code::kTemporaryRedirect, "307"},
        {ziapi::http::Code::kBadRequest, "400"},
        {ziapi::http::Code::kUnauthorized, "401"},
        {ziapi::http::Code::kPaymentRequired, "402"},
        {ziapi::http::Code::kForbidden, "403"},
        {ziapi::http::Code::kNotFound, "404"},
        {ziapi::http::Code::kMethodNotAllowed, "405"},
        {ziapi::http::Code::kNotAcceptable, "406"},
        {ziapi::http::Code::kProxyAuthenticationRequired, "407"},
        {ziapi::http::Code::kRequestTimeOut, "408"},
        {ziapi::http::Code::kConflict, "409"},
        {ziapi::http::Code::kGone, "410"},
        {ziapi::http::Code::kLengthRequired, "411"},
        {ziapi::http::Code::kPreconditionFailed, "412"},
        {ziapi::http::Code::kRequestEntityTooLarge, "413"},
        {ziapi::http::Code::kRequestURITooLarge, "414"},
        {ziapi::http::Code::kUnsupportedMediaType, "415"},
        {ziapi::http::Code::kRequestedRangeNotSatisfiable, "416"},
        {ziapi::http::Code::kExpectationFailed, "417"},
        {ziapi::http::Code::kInternalServerError, "500"},
        {ziapi::http::Code::kNotImplemented, "501"},
        {ziapi::http::Code::kBadGateway, "502"},
        {ziapi::http::Code::kServiceUnavailable, "503"},
        {ziapi::http::Code::kGatewayTime_out, "504"},
        {ziapi::http::Code::kHttpVersionNotSupported, "505"},
    };
}
#endif /* !HTTPMODULE_HPP_ */

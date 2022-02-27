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

namespace zia::modules::http {

    static const std::map<std::string, ziapi::http::Version> _versions {
        {"KV1", ziapi::http::Version::kV1},
        {"KV1_1", ziapi::http::Version::kV1_1},
        {"KV2", ziapi::http::Version::kV2},
        {"KV3", ziapi::http::Version::kV3},
        {"10", ziapi::http::Version::kV1},
        {"11", ziapi::http::Version::kV1_1},
        {"20", ziapi::http::Version::kV2},
        {"30", ziapi::http::Version::kV3},
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

    class HttpModule {
        public:
            HttpModule() = default;
            ~HttpModule() = default;

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

    static const std::vector<std::string> _headers {
        ziapi::http::header::kAIM,
        ziapi::http::header::kAccept,
        ziapi::http::header::kAcceptCharset,
        ziapi::http::header::kAcceptDatetime,
        ziapi::http::header::kAcceptEncoding,
        ziapi::http::header::kAcceptLanguage,
        ziapi::http::header::kAccessControlRequestMethod,
        ziapi::http::header::kAccessControlRequestHeaders,
        ziapi::http::header::kAuthorization,
        ziapi::http::header::kCacheControl,
        ziapi::http::header::kConnection,
        ziapi::http::header::kContentLength,
        ziapi::http::header::kContentMD5,
        ziapi::http::header::kContentType,
        ziapi::http::header::kCookie,
        ziapi::http::header::kDate,
        ziapi::http::header::kExpect,
        ziapi::http::header::kForwarded,
        ziapi::http::header::kFrom,
        ziapi::http::header::kHost,
        ziapi::http::header::kHTTP2Settings,
        ziapi::http::header::kIfMatch,
        ziapi::http::header::kIfModifiedSince,
        ziapi::http::header::kIfNoneMatch,
        ziapi::http::header::kIfRange,
        ziapi::http::header::kIfUnmodifiedSince,
        ziapi::http::header::kMaxForwards,
        ziapi::http::header::kOrigin,
        ziapi::http::header::kPragma,
        ziapi::http::header::kPrefer,
        ziapi::http::header::kProxyAuthorization,
        ziapi::http::header::kRange,
        ziapi::http::header::kReferer,
        ziapi::http::header::kTE,
        ziapi::http::header::kUserAgent,
        ziapi::http::header::kUpgrade,
        ziapi::http::header::kVia,
        ziapi::http::header::kAcceptCH,
        ziapi::http::header::kAccessControlAllowOrigin,
        ziapi::http::header::kAccessControlAllowCredentials,
        ziapi::http::header::kAccessControlExposeHeaders,
        ziapi::http::header::kAccessControlMaxAge,
        ziapi::http::header::kAccessControlAllowMethods,
        ziapi::http::header::kAccessControlAllowHeaders,
        ziapi::http::header::kAcceptPatch,
        ziapi::http::header::kAcceptRanges,
        ziapi::http::header::kAge,
        ziapi::http::header::kAllow,
        ziapi::http::header::kAltSvc,
        ziapi::http::header::kContentDisposition,
        ziapi::http::header::kContentEncoding,
        ziapi::http::header::kContentLanguage,
        ziapi::http::header::kContentLocation,
        ziapi::http::header::kContentRange,
        ziapi::http::header::kDeltaBase,
        ziapi::http::header::kETag,
        ziapi::http::header::kExpires,
        ziapi::http::header::kIM,
        ziapi::http::header::kLastModified,
        ziapi::http::header::kLink,
        ziapi::http::header::kLocation,
        ziapi::http::header::kP3P,
        ziapi::http::header::kPreferenceApplied,
        ziapi::http::header::kProxyAuthenticate,
        ziapi::http::header::kPublicKeyPins,
        ziapi::http::header::kRetryAfter,
        ziapi::http::header::kServer,
        ziapi::http::header::kSetCookie,
        ziapi::http::header::kStrictTransportSecurity,
        ziapi::http::header::kTrailer,
        ziapi::http::header::kTransferEncoding,
        ziapi::http::header::kTk,
        ziapi::http::header::kVary,
        ziapi::http::header::kWarning,
        ziapi::http::header::kWWWAuthenticate,
        ziapi::http::header::kXFrameOptions,
    };
}
#endif /* !HTTPMODULE_HPP_ */

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
#include <Exceptions/MyException.hpp>
#include "ziapi/Http.hpp"

namespace zia::modules::http {

    static const std::map<std::string, ziapi::http::Version> _versions {
        {"10", ziapi::http::Version::kV1},
        {"11", ziapi::http::Version::kV1_1},
        {"20", ziapi::http::Version::kV2},
        {"30", ziapi::http::Version::kV3},
        {"KV1", ziapi::http::Version::kV1},
        {"KV1_1", ziapi::http::Version::kV1_1},
        {"KV2", ziapi::http::Version::kV2},
        {"KV3", ziapi::http::Version::kV3},
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
            HttpModule();
            ~HttpModule();

            ziapi::http::Request createRequest(const std::string &str);
            std::string readResponse(ziapi::http::Response &res);

        protected:
        private:
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

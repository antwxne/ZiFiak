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
#include <algorithm>
#include <MyException.hpp>
#include "ziapi/Http.hpp"

namespace zia::modules::http {

    static const std::map<std::string, ziapi::http::Version> _versions {
        {"10", ziapi::http::Version::kV1},
        {"11", ziapi::http::Version::kV1_1},
        {"20", ziapi::http::Version::kV2},
        {"30", ziapi::http::Version::kV3}
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

            ziapi::http::Request &createRequest(std::string &str);
            std::string &readResponse(ziapi::http::Response &res);

        protected:
        private:
    };

    static const std::vector<std::string> _headers {
        kAIM ,
        kAccept,
        kAcceptCharset,
        kAcceptDatetime,
        kAcceptEncoding,
        kAcceptLanguage,
        kAccessControlRequestMethod,
        kAccessControlRequestHeaders,
        kAuthorization,
        kCacheControl,
        kConnection,
        kContentLength,
        kContentMD5,
        kContentType,
        kCookie,
        kDate,
        kExpect,
        kForwarded,
        kFrom,
        kHost,
        kHTTP2Settings,
        kIfMatch,
        kIfModifiedSince,
        kIfNoneMatch,
        kIfRange,
        kIfUnmodifiedSince,
        kMaxForwards,
        kOrigin,
        kPragma,
        kPrefer,
        kProxyAuthorization,
        kRange,
        kReferer,
        kTE,
        kUserAgent,
        kUpgrade,
        kVia,
        kAcceptCH,
        kAccessControlAllowOrigin,
        kAccessControlAllowCredentials,
        kAccessControlExposeHeaders,
        kAccessControlMaxAge,
        kAccessControlAllowMethods,
        kAccessControlAllowHeaders,
        kAcceptPatch,
        kAcceptRanges,
        kAge,
        kAllow,
        kAltSvc,
        kContentDisposition,
        kContentEncoding,
        kContentLanguage,
        kContentLocation,
        kContentRange,
        kDeltaBase,
        kETag,
        kExpires,
        kIM,
        kLastModified,
        kLink,
        kLocation,
        kP3P,
        kPreferenceApplied,
        kProxyAuthenticate,
        kPublicKeyPins,
        kRetryAfter,
        kServer,
        kSetCookie,
        kStrictTransportSecurity,
        kTrailer,
        kTransferEncoding,
        kTk,
        kVary,
        kWarning,
        kWWWAuthenticate,
        kXFrameOptions,
    };
}
#endif /* !HTTPMODULE_HPP_ */

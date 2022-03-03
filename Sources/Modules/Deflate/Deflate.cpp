/*
** EPITECH PROJECT, 2022
** ZiFiak
** File description:
** Deflate
*/

#include <iostream>
#include <string.h>
#include "Deflate.hpp"
#include "zlib.h"

Deflate::Deflate() : _activated(false)
{
}

Deflate::~Deflate()
{
}


void Deflate::Init([[maybe_unused]] const ziapi::config::Node &config)
{
    try {
        if (config["Deflate"]["activated"].AsBool()) {
            this->_activated = true;
        }
    } catch(const std::exception& e) {
    }
    
}

[[nodiscard]] ziapi::Version Deflate::GetVersion() const noexcept
{
    return ziapi::Version{5, 0, 0};
}

[[nodiscard]] ziapi::Version Deflate::GetCompatibleApiVersion() const noexcept
{
    return ziapi::Version{5, 0, 0};
}

[[nodiscard]] const char *Deflate::GetName() const noexcept
{
    return "Deflate Module";
}

[[nodiscard]] const char *Deflate::GetDescription() const noexcept
{
    return "Compress the response body before sending it back to the network";
}

void Deflate::PostProcess(ziapi::http::Context &context, const ziapi::http::Request &,ziapi::http::Response &res)
{
    res.body = this->_compressString(res.body);
    std::cout << res.body << std::endl;
    res.headers.insert({"Content-Encoding", "deflate"});
    if (res.headers.find("Content-Lenght") != res.headers.end()) {
        res.headers["Content-Length"] = res.body.size();
    } else {
        res.headers.insert({"Content-Length", std::to_string(res.body.size())});
    }
}

[[nodiscard]] double Deflate::GetPostProcessorPriority() const noexcept
{
    return 0.1f;
}

bool Deflate::ShouldPostProcess(const ziapi::http::Context &context, const ziapi::http::Request &req, const ziapi::http::Response &res) const
{
    if (!this->_activated)
        return false;
    if (req.headers.find("Accept-Encoding") != req.headers.end() && req.headers.at("Accept-Encoding").find("gzip") != std::string::npos)
        return true;
    return false;
}


std::string Deflate::_compressString(const std::string &str) const noexcept
{
    int compressedSize = (str.size() * 1.1) + 12;
    char *compressedString = (char *)malloc(compressedSize);

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    defstream.avail_in = (uInt)str.size();
    defstream.next_in = (Bytef *)str.c_str();
    defstream.avail_out = (uInt)compressedSize;
    defstream.next_out = (Bytef *)compressedString;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    // printf("Compressed size is: %lu\n", defstream.total_out);
    // printf("Compressed string is: %.*s\n", defstream.total_out, compressedString);
    // printf("\n----------\n\n");

    std::string cppStr(compressedString, defstream.total_out);
    free(compressedString);
    return cppStr;
}

std::string Deflate::_uncompressString(const std::string &str) const noexcept
{
    std::size_t uncompressedSize = str.size() * 2;
    std::string uncompressedString(uncompressedSize, '\0');

    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.avail_in = (uInt)(str.size()); // size of input
    infstream.next_in = (Bytef *)str.c_str(); // input char array
    infstream.avail_out = (uInt)uncompressedString.size(); // size of output
    infstream.next_out = (Bytef *)uncompressedString.c_str(); // output char array
     
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);

    // printf("Uncompressed size is: %lu\n", infstream.total_out);
    // printf("Uncompressed string is: %.*s\n", infstream.total_out, uncompressedString.c_str());
    // printf("\n----------\n\n");

    uncompressedString.erase(std::remove(uncompressedString.begin(), uncompressedString.end(), '\0'), uncompressedString.end());
    return uncompressedString;
}

DYLIB_API ziapi::IModule *LoadZiaModule()
{
    return new Deflate;
}

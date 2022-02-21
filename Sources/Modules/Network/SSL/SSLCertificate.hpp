//
// Created by Clement Ruat on 18/02/2022.
//

#pragma once
#include <string>

class SSLCertificate {
    SSLCertificate() = default;
    ~SSLCertificate() = default;
    void verifyCertificateClient();
    void verifyCertificateServer();
};
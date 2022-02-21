set(BASE_MODULE
        Sources/Exceptions/MyException.cpp
        Sources/Exceptions/MyException.hpp
        Sources/Debug/Debug.hpp
        Sources/Debug/Debug.cpp
       )

# BASIC NETWORK MODULE
add_library(basic_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/Basic/BasicNetwork.cpp
        Sources/Modules/Network/Basic/BasicNetwork.hpp
        Sources/Modules/Network/Basic/Client.hpp
        Sources/Modules/Network/Basic/Client.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        )
conan_target_link_libraries(basic_network ${CONAN_LIBS})
add_dependencies(basic_network ziapi)


# SSL NETWORK MODULE
add_library(SSL_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/SSL/SSLClient.hpp
        Sources/Modules/Network/SSL/SSLClient.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        Sources/Modules/Network/SSL/SSLCertificate.hpp
        Sources/Modules/Network/SSL/SSLCertificate.cpp
        )
conan_target_link_libraries(SSL_network ${CONAN_LIBS})
add_dependencies(SSL_network ziapi)
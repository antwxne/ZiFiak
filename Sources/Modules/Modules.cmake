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
        Sources/Modules/Http/HttpModule.cpp Sources/Modules/Http/HttpModule.hpp
        )
conan_target_link_libraries(basic_network ${CONAN_LIBS})
add_dependencies(basic_network ziapi)

# Allow Methods MODULE
add_library(allow_method SHARED ${BASE_MODULE}
        Sources/Modules/AllowMethods/AllowMethods.hpp
        Sources/Modules/AllowMethods/AllowMethods.cpp
        )
conan_target_link_libraries(allow_method ${CONAN_LIBS})
add_dependencies(allow_method ziapi)

# Fall Back MODULE
add_library(fall_back SHARED ${BASE_MODULE}
        Sources/Modules/FallBack/FallBack.hpp
        Sources/Modules/FallBack/FallBack.cpp
        )
conan_target_link_libraries(fall_back ${CONAN_LIBS})
add_dependencies(fall_back ziapi)

# Php-cgi MODULE
add_library(php_cgi SHARED ${BASE_MODULE}
        Sources/Modules/Php/PhpModule.hpp
        Sources/Modules/Php/PhpModule.cpp
        )
conan_target_link_libraries(php_cgi ${CONAN_LIBS})
add_dependencies(php_cgi ziapi)
# Delegate MODULE
add_library(deflate SHARED ${BASE_MODULE}
        Sources/Modules/Deflate/Deflate.hpp
        Sources/Modules/Deflate/Deflate.cpp
        )
conan_target_link_libraries(deflate ${CONAN_LIBS})
add_dependencies(deflate ziapi)

# SSL NETWORK MODULE
add_library(SSL_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/SSL/SSLClient.hpp
        Sources/Modules/Network/SSL/SSLClient.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        Sources/Modules/Network/SSL/SslNetwork.cpp
        Sources/Modules/Network/SSL/SslNetwork.hpp
        Sources/Modules/Http/HttpModule.cpp Sources/Modules/Http/HttpModule.hpp
        )
conan_target_link_libraries(SSL_network ${CONAN_LIBS})
add_dependencies(SSL_network ziapi)

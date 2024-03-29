set(BASE_MODULE
        Sources/Exceptions/MyException.cpp
        Sources/Exceptions/MyException.hpp
        Sources/Debug/Debug.hpp
        Sources/Debug/Debug.cpp
       )

# HTTP NETWORK MODULE
add_library(HTTP_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/HTTP/HTTPNetwork.cpp
        Sources/Modules/Network/HTTP/HTTPNetwork.hpp
        Sources/Modules/Network/HTTP/HTTPClient.hpp
        Sources/Modules/Network/HTTP/HTTPClient.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        Sources/Modules/Network/HTTPParser/HTTPParser.cpp
        Sources/Modules/Network/HTTPParser/HTTPParser.hpp
        )
conan_target_link_libraries(HTTP_network ${CONAN_LIBS})
add_dependencies(HTTP_network ziapi)

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
# Global-cgi MODULE
add_library(global_cgi SHARED ${BASE_MODULE}
        Sources/Modules/Global/GlobalModule.hpp
        Sources/Modules/Global/GlobalModule.cpp
        )
conan_target_link_libraries(global_cgi ${CONAN_LIBS})
add_dependencies(global_cgi ziapi)
# Delegate MODULE
add_library(deflate SHARED ${BASE_MODULE}
        Sources/Modules/Deflate/Deflate.hpp
        Sources/Modules/Deflate/Deflate.cpp
        )
conan_target_link_libraries(deflate ${CONAN_LIBS})
add_dependencies(deflate ziapi)

# Log MODULE
add_library(log SHARED ${BASE_MODULE}
        Sources/Modules/Log/Log.hpp
        Sources/Modules/Log/Log.cpp
        )
conan_target_link_libraries(log ${CONAN_LIBS})
add_dependencies(log ziapi)

# SetID MODULE
add_library(setId SHARED ${BASE_MODULE}
        Sources/Modules/SetID/SetID.hpp
        Sources/Modules/SetID/SetID.cpp
        )
conan_target_link_libraries(setId ${CONAN_LIBS})
add_dependencies(setId ziapi)

# Environment MODULE
add_library(Environment SHARED ${BASE_MODULE}
        Sources/Modules/Environment/Environment.hpp
        Sources/Modules/Environment/Environment.cpp
        )
conan_target_link_libraries(Environment ${CONAN_LIBS})
add_dependencies(Environment ziapi)

# HTTPS NETWORK MODULE
add_library(HTTPS_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/HTTPS/HTTPSClient.hpp
        Sources/Modules/Network/HTTPS/HTTPSClient.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        Sources/Modules/Network/HTTPS/HTTPSNetwork.cpp
        Sources/Modules/Network/HTTPS/HTTPSNetwork.hpp
        Sources/Modules/Network/HTTPParser/HTTPParser.cpp
        Sources/Modules/Network/HTTPParser/HTTPParser.hpp
        )
conan_target_link_libraries(HTTPS_network ${CONAN_LIBS})
add_dependencies(HTTPS_network ziapi)

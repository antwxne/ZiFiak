set(BASE_MODULE
        Sources/Exceptions/MyException.cpp
        Sources/Exceptions/MyException.hpp
        Sources/Debug/Debug.hpp
        Sources/Debug/Debug.cpp
        Sources/dylib/dylib.hpp
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

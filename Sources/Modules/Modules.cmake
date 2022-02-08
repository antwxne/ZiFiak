set(BASE_MODULE
        build/ziapi-prefix/src/ziapi/include/ziapi/Module.hpp
        build/ziapi-prefix/src/ziapi/include/ziapi/Http.hpp
        build/ziapi-prefix/src/ziapi/include/ziapi/HttpConstants.hpp
        build/ziapi-prefix/src/ziapi/include/ziapi/Version.hpp
        Sources/Exceptions/MyException.cpp
        Sources/Exceptions/MyException.hpp
        Sources/Debug/Debug.hpp
        Sources/Debug/Debug.cpp
        )

# BASIC NETWORK MODULE
add_library(basic_network SHARED ${BASE_MODULE}
        Sources/Modules/Network/BasicNetwork.cpp
        Sources/Modules/Network/BasicNetwork.hpp
        Sources/Modules/Network/Client.hpp
        Sources/Modules/Network/Client.cpp
        )
conan_target_link_libraries(basic_network ${CONAN_LIBS})

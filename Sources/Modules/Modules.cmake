set(BASE_MODULE
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
add_dependencies(basic_network ziapi)

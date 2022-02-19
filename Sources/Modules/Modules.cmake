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

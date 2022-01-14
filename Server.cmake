

#Sources SERVER
set(SRC
        Sources/Server/Server.cpp Sources/Server/Server.hpp
        Sources/Server/Client.cpp Sources/Server/Client.hpp
        Sources/Debug/Debug.cpp Sources/Debug/Debug.hpp
        Sources/Exceptions/MyException.cpp Sources/Exceptions/MyException.hpp
        API/Api.cpp API/Api.hpp
        API/IModule.hpp
        Sources/Server/Config/Configuration.cpp Sources/Server/Config/Configuration.hpp)



#Sources tests unitaires

set(TESTS_SRC
        ${SRC})
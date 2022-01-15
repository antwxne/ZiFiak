#Sources SERVER
set(SRC
        Sources/Server/Server.cpp Sources/Server/Server.hpp
        Sources/Server/Client.cpp Sources/Server/Client.hpp
        Sources/Debug/Debug.cpp Sources/Debug/Debug.hpp
        Sources/Exceptions/MyException.cpp Sources/Exceptions/MyException.hpp
        API/Api.cpp API/Api.hpp
        Sources/Server/Config/Configuration.cpp Sources/Server/Config/Configuration.hpp
        Sources/HTTP/Request/Request.cpp Sources/HTTP/Request/Request.hpp
        Sources/HTTP/Response/Response.cpp Sources/HTTP/Response/Response.hpp
        Sources/Watcher/Watcher.cpp Sources/Watcher/Watcher.hpp
        API/IModule.hpp
        API/AModule.cpp API/AModule.hpp
        )



#Sources tests unitaires

set(TESTS_SRC
        ${SRC})
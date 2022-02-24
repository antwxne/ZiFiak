#Sources SERVER
set(SRC
        Sources/Server/Server.cpp Sources/Server/Server.hpp
        Sources/Debug/Debug.cpp Sources/Debug/Debug.hpp
        Sources/Exceptions/MyException.cpp Sources/Exceptions/MyException.hpp
        Sources/LoadLibs/LoadLibs.cpp Sources/LoadLibs/LoadLibs.hpp
        Sources/dylib/dylib.hpp
        Sources/Server/ConfigParser.cpp Sources/Server/ConfigParser.hpp
        Sources/Queue/RequestQueue.cpp Sources/Queue/RequestQueue.hpp
        Sources/Queue/ResponseQueue.cpp Sources/Queue/ResponseQueue.hpp
        Sources/Modules/Http/HttpModule.cpp Sources/Modules/Http/HttpModule.hpp
        Sources/Watcher/Watcher.cpp Sources/Watcher/Watcher.hpp
        )

set(SRC ${SRC}
        Sources/Modules/Network/Basic/BasicNetwork.cpp
        Sources/Modules/Network/Basic/BasicNetwork.hpp
        Sources/Modules/Network/Basic/Client.hpp
        Sources/Modules/Network/Basic/Client.cpp
        Sources/Modules/Network/AClient.hpp
        Sources/Modules/Network/AClient.cpp
        Sources/Modules/Http/HttpModule.cpp
        Sources/Modules/Http/HttpModule.hpp
        )
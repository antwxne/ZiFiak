#Cmake version
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Project name
project(Zia)

#Flags
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wextra -ggdb -g3")


if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message("Defining DEBUG Macro...")
    add_compile_definitions(DEBUG)
endif ()

#Bin name
set(NAME zia)

#Truc conan
include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup(TARGETS KEEP_RPATHS)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

#include
include_directories(Sources)

#Sources
set(SRC Sources/Server/Server.cpp Sources/Server/Server.hpp
        Sources/Server/Client.cpp Sources/Server/Client.hpp 
        Sources/Debug/Debug.cpp Sources/Debug/Debug.hpp
        Sources/Exceptions/MyException.cpp Sources/Exceptions/MyException.hpp
        )

set(MAIN Sources/Main.cpp )


set(TESTS_SRC
        ${SRC})

if(NOT UNIT_TESTS STREQUAL TRUE)

    add_executable(${NAME} ${MAIN} ${SRC})
    conan_target_link_libraries(${NAME} ${CONAN_LIBS})

else()
    set(TESTS_NAME unit_tests)
    set(CMAKE_CXX_FLAGS --coverage)

    add_executable(${TESTS_NAME} ${TESTS_SRC})
    find_library(CRITERION criterion)
    target_link_libraries(${TESTS_NAME} ${CRITERION})
    conan_target_link_libraries(${TESTS_NAME} ${CONAN_LIBS})
endif()
#Cmake version
cmake_minimum_required(VERSION 3.21)

#Project name
project(Zia)

#Bin name
set(NAME zia)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Flags
set(CMAKE_CXX_FLAGS_RELEASE_INIT -Wall -Wextra)
set(CMAKE_CXX_FLAGS_DEBUG_INIT ${CMAKE_CXX_FLAGS_RELEASE_INIT} -ggdb -g3)


if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message("Defining DEBUG Macro...")
    add_compile_definitions(DEBUG)
endif ()


#Truc conan
include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup(TARGETS KEEP_RPATHS)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Change le dossier d'ouput des binaires / .so
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/Modules)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/Modules)

#includes
include_directories(Sources)
include_directories(API)

# Server Sources
include(Sources/Server.cmake)
set(MAIN
        Sources/Main.cpp)
set(SRC ${SRC})
include(Sources/Modules/Modules.cmake)


# Build le server
add_executable(${NAME} ${MAIN} ${SRC})
conan_target_link_libraries(${NAME} ${CONAN_LIBS})

if(UNIX)
    target_link_libraries(${NAME} dl)
endif()

# Truc pour utiliser l'api de reference
include(ExternalProject)

ExternalProject_Add(
        ziapi
        GIT_REPOSITORY https://github.com/martin-olivier/ZiAPI.git
        GIT_TAG v3.0.0
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)

add_dependencies(${NAME} ziapi)
ExternalProject_Get_Property(ziapi SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)


#truc pour faire des TU avec google tests
#if (CMAKE_BUILD_TYPE STREQUAL Debug)
#    include(Tests/Tests.cmake)
#endif ()

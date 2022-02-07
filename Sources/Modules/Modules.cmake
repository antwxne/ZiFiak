set(MODULE1_NAME testModule.cpp)
set(MODULE1_SRC
        Sources/Modules/testModule.cpp)
add_library(${MODULE1_NAME} SHARED ${MODULE1_SRC})
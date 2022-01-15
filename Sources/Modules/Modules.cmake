# Interface et Abstraction des modules
set(BASE_MODULE
        API/IModule.hpp
        API/AModule.cpp API/AModule.hpp)

#Module d'exemple
set(MODULE1_NAME module1)
set(MODULE1_SRC
        Sources/Modules/TestModule.cpp Sources/Modules/TestModule.hpp)
add_library(${MODULE1_NAME} SHARED ${MODULE1_SRC} ${BASE_MODULE})


#Ajoute les modules aux TU
set(TESTS_SRC
        ${TESTS_SRC}
        ${MODULE1_SRC} )

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TESTS_SRC

        )

enable_testing()

set(CMAKE_CXX_FLAGS --coverage)
add_executable(${TESTS_NAME} ${TESTS_SRC} ${SRC})

target_link_libraries(${TESTS_NAME} gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})
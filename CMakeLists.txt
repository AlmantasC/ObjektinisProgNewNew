cmake_minimum_required(VERSION 3.14)
project(ObjektinisProg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CONTAINER "vector" CACHE STRING "Container type (vector, list, deque, myvector)")
set_property(CACHE CONTAINER PROPERTY STRINGS vector list deque myvector)
string(TOUPPER ${CONTAINER} CONTAINER_UPPER)

set(SOURCES
    src/main.cpp
    src/mylib.cpp
)

add_executable(${CONTAINER} ${SOURCES})
target_include_directories(${CONTAINER} PRIVATE src)
target_compile_definitions(${CONTAINER} PRIVATE USE_${CONTAINER_UPPER})

option(BUILD_BENCHMARK "Build Vector benchmark" OFF)
if(BUILD_BENCHMARK)
    add_executable(benchmark benchmark.cpp)
    target_include_directories(benchmark PRIVATE src)
    target_compile_options(benchmark PRIVATE -O2)
endif()

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    # studentas klasės testai (esami)
    add_executable(tests
        test/main.cpp
        src/mylib.cpp
    )
    target_include_directories(tests PRIVATE src)
    target_compile_definitions(tests PRIVATE USE_${CONTAINER_UPPER})
    target_link_libraries(tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(tests)

    # Vector klasės testai (nauji)
    add_executable(vector_tests
        test/vector_test.cpp
    )
    target_include_directories(vector_tests PRIVATE src)
    target_link_libraries(vector_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(vector_tests)

    install(TARGETS tests vector_tests RUNTIME DESTINATION bin)
endif()

include(GoogleTest)

cmake_minimum_required(VERSION 3.10)

project(spice_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GTEST_ROOT "/usr/local/include/gtest" CACHE PATH "Path to googletest")
find_package(GTest REQUIRED)

file(GLOB SRC
    src/*.h
    src/*.cpp
)

file(GLOB TESTS
    tests/*.h
    tests/*.cpp
)

add_executable(${PROJECT_NAME} "src/main.cpp" ${SRC})

add_executable(tests ${TESTS})
target_link_libraries(tests GTest::GTest)
gtest_discover_tests(tests)

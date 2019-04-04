include(GoogleTest)

cmake_minimum_required(VERSION 3.10)

project(spice_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GTEST_ROOT "/usr/local/include/gtest" CACHE PATH "Path to googletest")
find_package(GTest REQUIRED)

# Main executable config
file(GLOB SRC
    src/**/*.h
    src/**/*.hpp
    src/**/*.cpp
)
add_executable(${PROJECT_NAME} "src/main.cpp" ${SRC})

# Test executable config
file(GLOB TESTS
    tests/*.cpp
    tests/**/*.cpp
)
add_executable(tests ${TESTS})
target_link_libraries(tests GTest::GTest)
gtest_discover_tests(tests)

if(MSVC)
  add_compile_options(/W4 /WX)
else(MSVC)
  # add_compile_options(-Wall -Wextra -pedantic -Werror)
  add_compile_options(-Wall -Wextra -pedantic)
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -o1 -fno-omit-frame-pointer -fsanitize=address")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -o1 -fno-omit-frame-pointer -fsanitize=address")
endif(MSVC)

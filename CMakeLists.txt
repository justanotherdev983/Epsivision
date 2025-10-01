set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
cmake_minimum_required(VERSION 3.10)
project(Epsivision)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_SOURCE_DIR}/libs)

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
)

add_executable(epsivision ${SOURCES})

target_link_libraries(epsivision PRIVATE raylib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g3 ")

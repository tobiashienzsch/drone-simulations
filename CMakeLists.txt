cmake_minimum_required(VERSION 3.24)
project(drone-math-dev LANGUAGES CXX)

find_program(CCACHE ccache)
if (CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(mp-units REQUIRED)

add_executable(drone-math)
target_sources(drone-math PRIVATE src/main.cpp)
target_link_libraries(drone-math PRIVATE mp-units::mp-units)
target_compile_options(drone-math PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Werror")

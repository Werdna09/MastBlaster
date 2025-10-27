cmake_minimum_required(VERSION 3.10)
project(Mathmosaic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Mathmosaic ${SOURCES})
target_link_libraries(Mathmosaic raylib)


cmake_minimum_required(VERSION 3.10)
project(Dungeon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_SOURCE_DIR}/include)


file(GLOB SOURCES "source/*.cpp")
add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

message(STATUS "Source files:")
foreach(source ${SOURCES})
    message(STATUS "   -- ${source}")
endforeach()
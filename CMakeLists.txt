cmake_minimum_required(VERSION 3.0.0)
project(gdb_symbol_read VERSION 0.1.0)
set(CMAKE_BUILD_TYPE "Release")
include(CTest)
enable_testing()

add_executable(gdb_symbol_read main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

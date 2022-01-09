cmake_minimum_required(VERSION 3.20)
project(emucpu)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(emucpu_exe main.cpp)
add_library(emucpu emucpu.cpp)

include(FetchContent)
include(CTest)
include(cmake/CPM.cmake)
include(cmake/fetch-googletest.cmake)
include(cmake/fetch-fmt.cmake)

enable_testing()

add_subdirectory(test)
add_subdirectory(lib)

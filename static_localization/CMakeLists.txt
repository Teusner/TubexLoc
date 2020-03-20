cmake_minimum_required(VERSION 3.0.2)
project(TUBEX_BASICS_01 LANGUAGES CXX)

# Compilation options
set(CMAKE_CXX_STANDARD 11)

# Dependencies
include(FindPkgConfig)
pkg_search_module(TUBEX REQUIRED tubex)
message(STATUS "Found Tubex version ${TUBEX_VERSION}")

# Compilation
add_executable(my_project src/main.cpp)
target_compile_options(my_project PUBLIC ${TUBEX_CFLAGS})
target_include_directories(my_project PUBLIC ${TUBEX_INCLUDE_DIRS})
target_link_libraries(my_project PUBLIC ${TUBEX_LDFLAGS})
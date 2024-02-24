cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME AnyEngine)
project(${TARGET_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(3rd)
add_subdirectory(modules)
add_subdirectory(source)



cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME AnyEngineProject)
project(${TARGET_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(3rd)
add_subdirectory(modules)
add_subdirectory(source)

if(WIN32)
   # add_subdirectory(runtime-src/proj.win32)
elseif(UNIX)
    if(APPLE)
        add_subdirectory(runtime-src/proj.mac)
    else()
        add_subdirectory(runtime-src/proj.android)
    endif()
else()
  message(FATAL_ERROR "Unknown Platform")
endif()


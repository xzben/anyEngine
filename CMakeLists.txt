cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME AnyEngineProject)
project(${TARGET_NAME} VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(3rd)
add_subdirectory(modules)
add_subdirectory(source)

message("Operating System: ${CMAKE_SYSTEM_NAME}")
message("Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message("C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message("C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    message("64-bit architecture")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    message("ARM architecture")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message("64-bit system")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message("32-bit system")
endif()

if(WIN32)
    message("This is Linux")
    add_subdirectory(runtime-src/proj.win32)
elseif(UNIX AND NOT APPLE)
    message("This is Linux")
    include("cmake/linux_config.cmake")
elseif(APPLE)
  # 检测 SDK 路径是否包含 "iPhone" 字样
  if(CMAKE_OSX_SYSROOT MATCHES "iPhoneOS.platform")
    message("Targeting iOS")
    add_subdirectory(runtime-src/proj.ios)
  elseif(CMAKE_OSX_SYSROOT MATCHES "MacOSX.platform")
    message("Targeting macOS")
    add_subdirectory(runtime-src/proj.mac)
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_subdirectory(runtime-src/proj.android)
endif()



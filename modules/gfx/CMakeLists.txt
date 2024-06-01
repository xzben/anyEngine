cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME gfx)

# Global flag to cause add_library() to create shared libraries if on.
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(GFX_LIB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(GFX_LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(COMMON_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../common/include")

file(GLOB_RECURSE INTERFACE_HEADERS CONFIGURE_DEPENDS ${GFX_LIB_INCLUDE}/*.h)
file(GLOB_RECURSE INTERFACE_SOURCES CONFIGURE_DEPENDS ${GFX_LIB_INCLUDE}/*.cpp)

file(GLOB_RECURSE IMPL_HEADERS CONFIGURE_DEPENDS ${GFX_LIB_SOURCE}/*.h)
file(GLOB_RECURSE IMPL_SOURCES CONFIGURE_DEPENDS ${GFX_LIB_SOURCE}/*.cpp)
if(APPLE)
file(GLOB_RECURSE IML_MM_SOURCES CONFIGURE_DEPENDS ${GFX_LIB_SOURCE}/*.mm)
else()
set(IML_MM_SOURCES "")
endif()


set(GFX_PRIVATE_INCLUDE 
${GFX_LIB_SOURCE}
${COMMON_MODULE_DIR}
${GFX_LIB_INCLUDE}
)

set(GFX_PUBLIC_INCLUDE
${GFX_LIB_INCLUDE}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
	${INTERFACE_HEADERS} 
	${INTERFACE_SOURCES}
  ${IMPL_HEADERS}
  ${IMPL_SOURCES}
  ${IML_MM_SOURCES})

add_library(${TARGET_NAME} ${INTERFACE_HEADERS} 
${INTERFACE_SOURCES}
${IMPL_HEADERS}
${IMPL_SOURCES}
${IML_MM_SOURCES})

add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

find_package(Vulkan REQUIRED)
set(VMA_STATIC_VULKAN_FUNCTIONS    OFF CACHE BOOL "Link statically with Vulkan API" FORCE)
set(VMA_DYNAMIC_VULKAN_FUNCTIONS   ON CACHE BOOL "Fetch pointers to Vulkan functions internally (no static linking)" FORCE)
set(VMA_BUILD_DOCUMENTATION        OFF CACHE BOOL "Create and install the HTML based API documentation" FORCE)
set(VMA_BUILD_SAMPLES              OFF CACHE BOOL "Fetch pointers to Vulkan functions internally (no static linking)" FORCE)
set(PROJECT_IS_TOP_LEVEL           OFF CACHE BOOL "" FORCE)
add_subdirectory(3rd/VulkanMemoryAllocator)

set(SPIRV_REFLECT_EXECUTABLE  OFF CACHE BOOL "Build spirv-reflect executable" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL    "Build a SPIRV-Reflect static library" FORCE)
set(SPIRV_REFLECT_BUILD_TESTS OFF CACHE BOOL  "Build the SPIRV-Reflect test suite" FORCE)
set(SPIRV_REFLECT_ENABLE_ASSERTS OFF CACHE BOOL "Enable asserts for debugging" FORCE)
set(SPIRV_REFLECT_ENABLE_ASAN OFF CACHE BOOL   "Use address sanitization" FORCE)

add_subdirectory(3rd/SPIRV-Reflect)

set(PLATFORM_LIBS "")

if(WIN32)
  include("cmake/window_config.cmake")
elseif(UNIX AND NOT APPLE)
    message("This is Linux")
    include("cmake/linux_config.cmake")
elseif(APPLE)
  # 检测 SDK 路径是否包含 "iPhone" 字样
  if(CMAKE_OSX_SYSROOT MATCHES "iPhoneOS.platform")
    message("Targeting iOS")
    include("cmake/ios_config.cmake")
  elseif(CMAKE_OSX_SYSROOT MATCHES "MacOSX.platform")
    message("Targeting macOS")
    include("cmake/mac_config.cmake")
  else()
    message("Unknown Apple platform")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
  include("cmake/android_config.cmake")
endif()




message("platofrm libs: ${PLATFORM_LIBS} platform include ${PLATFORM_INCLUDE}")



target_link_libraries(${TARGET_NAME} PRIVATE glew VulkanMemoryAllocator spirv-reflect-static Vulkan::Vulkan ${PLATFORM_LIBS})

target_include_directories(${TARGET_NAME} PUBLIC ${GFX_PUBLIC_INCLUDE})
target_include_directories(${TARGET_NAME} PRIVATE ${GFX_PRIVATE_INCLUDE} ${PLATFORM_INCLUDE})

cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME gfx)

set(GFX_LIB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../3rd/")
set(MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")

file(GLOB_RECURSE RUNTIME_HEADERS CONFIGURE_DEPENDS ${GFX_LIB_SOURCE}/*.h)
file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDS ${GFX_LIB_SOURCE}/*.cpp)

set(GFX_PRIVATE_INCLUDE 
${GFX_LIB_SOURCE}
${THIRD_LIB_DIR}
${THIRD_LIB_DIR}/SPIRV-Reflect/
${MODULES_DIR}/common/
${THIRD_LIB_DIR}/VulkanSDK/include/
${THIRD_LIB_DIR}/vulkanmemoryallocator/include/
)

set(GFX_PUBLIC_INCLUDE
${GFX_LIB_SOURCE}
${MODULES_DIR}/common/
)

source_group(TREE "${GFX_LIB_SOURCE}" FILES 
	${RUNTIME_HEADERS} 
	${RUNTIME_SOURCES})

source_group(TREE "${THIRD_LIB_DIR}" FILES 
	${RUNTIME_GLAB} 
	${RUNTIME_GLAB_HEADS})

add_definitions (-DGLEW_STATIC)
add_library(${TARGET_NAME} ${RUNTIME_HEADERS} 
	${RUNTIME_SOURCES}
    ${RUNTIME_GLAB})

target_include_directories(${TARGET_NAME} PUBLIC ${GFX_PUBLIC_INCLUDE})
target_include_directories(${TARGET_NAME} PRIVATE ${GFX_PRIVATE_INCLUDE})


find_package(Vulkan REQUIRED)

if(WIN32)
    set(vulkan_lib ${THIRD_LIB_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib)
    set(glslangValidator_executable ${THIRD_LIB_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe)
    set(spirv_cross_executable ${ENGINE_TOOL_DIR}/spirv-cross/win32/spirv-cross.exe)
    add_compile_definitions("ENGINE_VK_LAYER_PATH=${THIRD_LIB_DIR}/VulkanSDK/bin/Win32")
elseif(UNIX)
    if(APPLE)
      set(vulkan_lib ${THIRD_LIB_DIR}/VulkanSDK/lib/MacOS/libvulkan.1.dylib)
      set(glslangValidator_executable ${THIRD_LIB_DIR}/VulkanSDK/bin/MacOS/glslangValidator)
      add_compile_definitions("ENGINE_VK_LAYER_PATH=${THIRD_LIB_DIR}/VulkanSDK/bin/MacOS")
      add_compile_definitions("ENGINE_VK_ICD_FILENAMES=${THIRD_LIB_DIR}/VulkanSDK/bin/MacOS/MoltenVK_icd.json")
    else()
      set(vulkan_lib ${THIRD_LIB_DIR}/VulkanSDK/lib/Linux/libvulkan.so.1)
      set(glslangValidator_executable ${THIRD_LIB_DIR}/VulkanSDK/bin/Linux/glslangValidator)
      add_compile_definitions("ENGINE_VK_LAYER_PATH=${THIRD_LIB_DIR}/VulkanSDK/bin/Linux")
    endif()
else()
  message(FATAL_ERROR "Unknown Platform")
endif()

target_link_libraries(${TARGET_NAME} PRIVATE glfw glew ${vulkan_lib} spirv-reflect-static)
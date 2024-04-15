set(THIRD_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PART_FOLDER "3rd")

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)

    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${THIRD_PART_FOLDER}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${THIRD_PART_FOLDER}/glfw)
endif()

if(NOT TARGET stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES FOLDER ${THIRD_PART_FOLDER}/stb)
endif()

if(NOT TARGET glm)
    include(glm.cmake)
    set_target_properties(glm PROPERTIES FOLDER ${THIRD_PART_FOLDER}/glm)
endif()

if(NOT TARGET tinyobjloader)
    include(tinyobjloader.cmake)
    set_target_properties(tinyobjloader PROPERTIES FOLDER ${THIRD_PART_FOLDER}/tinyobjloader)
endif()

if(NOT TARGET glew)
    add_subdirectory(glew)
endif()

set(SPIRV_REFLECT_EXECUTABLE OFF CACHE BOOL "Build spirv-reflect executable" FORCE)
set(SPIRV_REFLECT_EXAMPLES OFF CACHE BOOL "Build stripper examples" FORCE)
set(SPIRV_REFLECT_STATIC_LIB ON CACHE BOOL "Build a SPIRV-Reflect static library" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SPIRV-Reflect)



if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)

    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${BYGFX_THIRD_PART_FOLDER}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${BYGFX_THIRD_PART_FOLDER}/glfw)
endif()

if(NOT TARGET stb)
    include(stb.cmake)
    set_target_properties(stb PROPERTIES FOLDER ${BYGFX_THIRD_PART_FOLDER}/stb)
endif()

if(NOT TARGET glm)
    include(glm.cmake)
    set_target_properties(glm PROPERTIES FOLDER ${BYGFX_THIRD_PART_FOLDER}/glm)
endif()

if(NOT TARGET tinyobjloader)
    include(tinyobjloader.cmake)
    set_target_properties(tinyobjloader PROPERTIES FOLDER ${BYGFX_THIRD_PART_FOLDER}/tinyobjloader)
endif()


set(THIRD_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
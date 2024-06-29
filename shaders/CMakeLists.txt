cmake_minimum_required(VERSION 3.0.0)

set(TARGET_NAME ShaderCompile)

project(${TARGET_NAME})

set(GLSL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glsl")
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS "${GLSL_SOURCE_DIR}/*.vert"
                                            "${GLSL_SOURCE_DIR}/*.frag"
                                            "${GLSL_SOURCE_DIR}/*.comp"
                                            "${GLSL_SOURCE_DIR}/*.geom"
                                            "${GLSL_SOURCE_DIR}/*.tesc"
                                            "${GLSL_SOURCE_DIR}/*.tese"
                                            "${GLSL_SOURCE_DIR}/*.mesh"
                                            "${GLSL_SOURCE_DIR}/*.task"
                                            "${GLSL_SOURCE_DIR}/*.rgen"
                                            "${GLSL_SOURCE_DIR}/*.rchit"
                                            "${GLSL_SOURCE_DIR}/*.rmiss"
                                            "${GLSL_SOURCE_DIR}/*.rcall")

SOURCE_GROUP("shaders" FILES ${SHADER_FILES})

set(SHADER_INCLUDE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GENERATED_SHADER_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../resources/shaders")

if (NOT EXISTS ${GENERATED_SHADER_FOLDER})
  file(MAKE_DIRECTORY ${GENERATED_SHADER_FOLDER})
endif()

include(cmake/ShaderCompile.cmake)

set(glslangValidator_executable ${CMAKE_CURRENT_SOURCE_DIR}/tools/glslangValidator.exe)
set(spirv_cross_executable ${CMAKE_CURRENT_SOURCE_DIR}/tools/spirv-cross.exe)

compile_shader(
  "${SHADER_FILES}"
  "${GLSL_SOURCE_DIR}"
  "${TARGET_NAME}"
  "${SHADER_INCLUDE_FOLDER}"
  "${GENERATED_SHADER_FOLDER}"
  "${glslangValidator_executable}"
  "${spirv_cross_executable}")

# For each shader, we create a header file
# foreach(SHADER ${shaders})
#     # Prepare a header name and a global variable for this shader
#     get_filename_component(SHADER_NAME ${SHADER} NAME)
#     string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME})
#     string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR)

#     set(SHADER_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_SHADER_FOLDER}/${HEADER_NAME}.h")

#     add_custom_target(
#       ${HEADER_NAME}
#       # Compile any GLSL shader into SPIR-V shader
#       # -g -Od should only be used in debug version
#       COMMAND ${glslangValidator_executable} -I${SHADER_INCLUDE_FOLDER} -V100 -o ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_SHADER_FOLDER}/${SHADER_NAME}.spv ${SHADER}
#       # Make a C header file with the SPIR-V shader
#       COMMAND ${CMAKE_COMMAND} -DPATH="${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_SHADER_FOLDER}/${SHADER_NAME}.spv" -DHEADER="${SHADER_HEADER}" -DGLOBAL="${GLOBAL_SHADER_VAR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/embed_resource.cmake"
#       # Rebuild the header file if the shader is updated
#       DEPENDS ${SHADER}
#       COMMENT "Building ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_SHADER_FOLDER}/${SHADER_NAME}.spv and embedding it into ${SHADER_HEADER}"
#     )

#     set_target_properties(${HEADER_NAME} PROPERTIES FOLDER "Shaders/generated" )
#     # Add the custom target like a dependencies of the project
#     add_dependencies(${PILOT_NAME} ${HEADER_NAME})

#     message(STATUS "Generating build commands for ${SHADER}")
# endforeach()


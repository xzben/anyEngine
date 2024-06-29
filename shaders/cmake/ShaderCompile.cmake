# 定义一个函数来获取相对路径
function(get_relative_path FROM_DIR TO_FILE OUTPUT_VAR)
    file(RELATIVE_PATH REL_PATH ${FROM_DIR} ${TO_FILE})
    set(${OUTPUT_VAR} ${REL_PATH} PARENT_SCOPE)
endfunction()

function(compile_shader SHADERS SHADER_ROOT TARGET_NAME SHADER_INCLUDE_FOLDER GENERATED_DIR GLSLANG_BIN SPIRVCROSS_BIN)

    set(working_dir "${CMAKE_CURRENT_SOURCE_DIR}")

    set(ALL_GENERATED_SPV_FILES "")
    set(ALL_GENERATED_CPP_FILES "")
    set(ALL_GENERATED_GLES3_FILES "")

    if(UNIX)
        execute_process(COMMAND chmod a+x ${GLSLANG_BIN})
        execute_process(COMMAND chmod a+x ${SPIRVCROSS_BIN})
    endif()

    execute_process(COMMAND rm -rf ${GENERATED_DIR}/*)
    foreach(SHADER ${SHADERS})
    # Prepare a header name and a global variable for this shader
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        get_filename_component(SHADER_PATH ${SHADER} PATH)

        string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME})
        string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR)

        get_relative_path(${SHADER_ROOT} ${SHADER_PATH} SHADER_RELATIVE_PATH)
        message("compile_shader" ${SHADER} ${SHADER_RELATIVE_PATH})

        set(SPV_FILE  "${GENERATED_DIR}/spv/${SHADER_RELATIVE_PATH}/${SHADER_NAME}.spv")
        set(GLES3_FILE "${GENERATED_DIR}/gles3/${SHADER_RELATIVE_PATH}/${SHADER_NAME}.es3")
        set(CPP_FILE  "${GENERATED_DIR}/${SHADER_RELATIVE_PATH}/${HEADER_NAME}.h")

        add_custom_command(
            OUTPUT ${SPV_FILE}
            COMMAND ${GLSLANG_BIN} -I${SHADER_INCLUDE_FOLDER} -V100 -o ${SPV_FILE} ${SHADER}
            DEPENDS ${SHADER}
            WORKING_DIRECTORY "${working_dir}")

        list(APPEND ALL_GENERATED_SPV_FILES ${SPV_FILE})

        add_custom_command(
            OUTPUT ${GLES3_FILE}
            COMMAND ${SPIRVCROSS_BIN} --version 310 --es ${SPV_FILE} --output ${GLES3_FILE}
            DEPENDS ${SPV_FILE}
            WORKING_DIRECTORY "${working_dir}")

        list(APPEND ALL_GENERATED_GLES3_FILES ${GLES3_FILE})

        #[[
        add_custom_command(
            OUTPUT ${CPP_FILE}
            COMMAND ${CMAKE_COMMAND} -DPATH=${SPV_FILE} -DHEADER="${CPP_FILE}" 
                -DGLOBAL="${GLOBAL_SHADER_VAR}" -P "${ENGINE_ROOT_DIR}/cmake/GenerateShaderCPPFile.cmake"
            DEPENDS ${SPV_FILE}
            WORKING_DIRECTORY "${working_dir}")

        list(APPEND ALL_GENERATED_CPP_FILES ${CPP_FILE})
        #]]
    endforeach()

    add_custom_target(${TARGET_NAME}
        DEPENDS ${ALL_GENERATED_SPV_FILES} ${ALL_GENERATED_CPP_FILES} ${ALL_GENERATED_GLES3_FILES} SOURCES ${SHADERS})

endfunction()
set(TARGET_NAME glew)

set(GLEW_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_definitions (-DGLEW_NO_GLU)
add_definitions (-DGLEW_STATIC)
add_definitions (-DGLEW_BUILD)

include_directories (${GLEW_DIR}/include)

set (GLEW_PUBLIC_HEADERS_FILES 
${GLEW_DIR}/include/GL/wglew.h 
${GLEW_DIR}/include/GL/glew.h 
${GLEW_DIR}/include/GL/glxew.h
${GLEW_DIR}/include/GL/eglew.h
)
set (GLEW_SRC_FILES ${GLEW_DIR}/src/glew.c)

add_library (${TARGET_NAME} STATIC ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${GLEW_DIR}/include)
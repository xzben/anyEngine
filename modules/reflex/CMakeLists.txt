set(TARGET_NAME reflex)

set(REFLEX_LIB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(REFLEX_LIB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(COMMON_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../common/include")

file(GLOB_RECURSE REFLEX_HEADERS CONFIGURE_DEPENDS ${REFLEX_LIB_SOURCE}/*.h)
file(GLOB_RECURSE REFLEX_SOURCES CONFIGURE_DEPENDS ${REFLEX_LIB_SOURCE}/*.cpp)


source_group(TREE "${REFLEX_LIB_SOURCE}" FILES 
	${REFLEX_HEADERS} 
	${REFLEX_SOURCES})

set(REFLEX_PRIVATE_INCLUDE 
    ${REFLEX_LIB_INCLUDE}
    ${REFLEX_LIB_SOURCE}
    ${COMMON_MODULE_DIR}
)

set(REFLEX_PUBLIC_INCLUDE
    ${REFLEX_LIB_INCLUDE}
)

add_subdirectory(3rd)

add_library(${TARGET_NAME} ${REFLEX_HEADERS} ${REFLEX_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${REFLEX_PUBLIC_INCLUDE})
target_include_directories(${TARGET_NAME} PRIVATE ${REFLEX_PRIVATE_INCLUDE})

target_link_libraries(${TARGET_NAME} PRIVATE jsoncpp)
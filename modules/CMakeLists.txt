set(MODULES_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MODULES_PART_FOLDER "modules")

if(NOT TARGET gfx)
    add_subdirectory(gfx)
    set_target_properties(gfx PROPERTIES FOLDER ${MODULES_PART_FOLDER})
endif()

if(NOT TARGET pipeline)
    add_subdirectory(pipeline)
    set_target_properties(pipeline PROPERTIES FOLDER ${MODULES_PART_FOLDER})
endif()
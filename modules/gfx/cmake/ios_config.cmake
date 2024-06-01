

find_library(UIKIT_LIBRARY UIKit)
find_library(OPENGLES_LIBRARY OpenGLES)
find_library(COREVIDEO_LIBRARY CoreVideo)
find_library(QUARTZCORE_LIBRARY QuartzCore)


# 当前平台 需要加载的库
set(PLATFORM_LIBS 
    ${UIKIT_LIBRARY} 
    ${OPENGLES_LIBRARY} 
    ${COREVIDEO_LIBRARY} 
    ${QUARTZCORE_LIBRARY})

# 当前平台需要添加的 头文件路径
set(PLATFORM_INCLUDE "")

set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE YES
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)
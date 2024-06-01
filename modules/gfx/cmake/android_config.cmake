
# 查找 OpenGL ES 库
find_library(GLESv3-lib GLESv3)
find_library(log-lib log)
find_library(EGL-lib EGL)

# 当前平台 需要加载的库
set(PLATFORM_LIBS ${GLESv3-lib}
    ${log-lib}
    ${EGL-lib})

# 当前平台需要添加的 头文件路径
set(PLATFORM_INCLUDE "")

add_definitions (-DDGLEW_STATIC)
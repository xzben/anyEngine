# 查找 OpenGL 和 GLUT 库
find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)

# 当前平台 需要加载的库
set(PLATFORM_LIBS 
  # OpenGL::GL 
  # GLUT::GLUT
  ${OPENGL_LIBRARIES} "-framework Cocoa")

# 设置项目属性
set_target_properties(${TARGET_NAME} PROPERTIES
  MACOSX_BUNDLE YES
)
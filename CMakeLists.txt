cmake_minimum_required(VERSION 3.12)
project(OpenGL)

set(CMAKE_CXX_STANDARD 14)

set(GLEW_H /usr/local/Homebrew/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Homebrew/Cellar/glfw/3.4/include/GLFW)
set(GLAD_H /Users/apple/Desktop/vscode/gl/glad/include/glad)
set(KH_H /Users/apple/Desktop/vscode/gl/glad/include/KHR)
set(GLM_H /usr/local/Homebrew/Cellar/glm/1.0.1/include/glm)
set(UTILS_H ${PROJECT_SOURCE_DIR}/utils)

include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H} ${GLM_H} ${UTILS_H})

# 添加目标链接
set(GLEW_LINK /usr/local/Homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /usr/local/Homebrew/Cellar/glfw/3.4/lib/libglfw.3.4.dylib)
set(GLM_LINK /usr/local/Homebrew/Cellar/glm/1.0.1/lib/libglm.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLM_LINK})

# 执行编译命令
set(SOURCE_FILES "glad/src/glad.c" "utils/OpenGLUtils.h")
add_executable(OpenGL ${SOURCE_FILES} multiCube_.cpp)

# mac下这步很重要
if (APPLE)
    target_link_libraries(OpenGL "-framework OpenGL")
    target_link_libraries(OpenGL "-framework GLUT")
    target_link_libraries(OpenGL ${GLFW_LINK})
#    target_link_libraries(OpenGL "-framework Cocoa")
#    target_link_libraries(OpenGL "-framework IOKit")
endif()
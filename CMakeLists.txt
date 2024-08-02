cmake_minimum_required(VERSION 3.12)
project(glLab)

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
link_directories("/opt/homebrew/lib")
include_directories("/opt/homebrew/include")


find_library(Cocoa_Library Cocoa)
find_library (
        SOIL2_Library
        NAMES SOIL2 soil2 libsoil2 libsoil2-debug.a  # what to look for
        HINTS "/opt/homebrew/lib" # where to look
        NO_DEFAULT_PATH # do not search system default paths
)
# or more briefly
# find_library (SOIL2_Library SOIL2)
find_library(OpenGl_Library OpenGL)
find_library(GLUT_Library glut)
# check if we found the library
# message(STATUS "SOIL2_Library: [${SOIL2_Library}]")

if (NOT SOIL2_Library)
    message(SEND_ERROR "Did not find lib SOIL2")
endif() # need parenthesis here

# 执行编译命令
set(SOURCE_FILES "glad/src/glad.c" "utils/OpenGLUtils.h"
        utils/Utils.cpp)
add_executable(glLab ${SOURCE_FILES}
#        multiCube.cpp
#        multiCube_.cpp
#        oneCube.cpp
        myStack.cpp
        #        triangle.cpp
)

# mac下这步很重要
if (APPLE)
    target_link_libraries(glLab "-framework OpenGL")
    target_link_libraries(glLab "-framework GLUT")
    target_link_libraries(glLab ${GLFW_LINK})
    target_link_libraries(glLab ${SOIL2_Library})
#    target_link_libraries(glLab "-framework Cocoa")
#    target_link_libraries(glLab "-framework IOKit")
endif()
// Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <glew.h>
#include <fstream>
#include <sstream>
// #include<glad/glad.h>
using namespace std;


namespace OpenGLUtils {

    void printShaderLog(GLuint shader) {
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char *)malloc(len);
            glGetShaderInfoLog(shader, len, &chWrittn, log);
            std::cout << "Shader Info Log: " << log << std::endl;
            free(log);
        }
    }

    void printProgramLog(int prog) {
        int len = 0;
        int chWrittn = 0;
        char *log;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        if (len > 0) {
            log = (char *)malloc(len);
            glGetProgramInfoLog(prog, len, &chWrittn, log);
            std::cout << "Program Info Log: " << log << std::endl;
            free(log);
        }
    }

    bool checkOpenGLError() {
        bool foundError = false;
        int glErr = glGetError();
        while (glErr != GL_NO_ERROR) {
            std::cout << "glError: " << glErr << std::endl;
            foundError = true;
            glErr = glGetError();
        }
        return foundError;
    }

    GLuint createShaderProgram(GLuint vShader, GLuint fShader) {
        GLint vertCompiled;
        GLint fragCompiled;
        GLint linked;
        // . . .
        // 捕获编译着色器时的错误
        glCompileShader(vShader);
        checkOpenGLError();
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1) {
            std::cout << "vertex compilation failed" << std::endl;
            printShaderLog(vShader);
        }
        std::cout << "222222" << std::endl;
        glCompileShader(fShader);
        checkOpenGLError();
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
        if (fragCompiled != 1) {
            std::cout << "fragment compilation failed" << std::endl;
            printShaderLog(fShader);
        }

        // 捕获链接着色器时的错误
        GLuint vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);
        glLinkProgram(vfProgram);
        checkOpenGLError();
        glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
        if (linked != 1) {
            std::cout << "linking failed" << std::endl;
            printProgramLog(vfProgram);
        }
        std::cout << "333333" << std::endl;
        return vfProgram;
    }

    // std::string readShaderSource(const char *filePath) {
    //     std::string content; 
    //     ifstream fileStream(filePath, std::ios::in); 
    //     std::string line = ""; 
    //     while (!fileStream.eof()) { 
    //         getline(fileStream, line); 
    //         content.append(line + "\n"); 
    //         std::cout << "loading line" + line << std::endl;
    //     } 
    //     std::cout << "content" + content << std::endl;
    //     fileStream.close(); 
    //     return content;
    // }

    std::string readShaderSource(const char* filePath) {
        std::string shaderSource;
        std::ifstream fileStream(filePath, std::ios::in);

        if (fileStream.is_open()) {
            std::stringstream buffer;
            buffer << fileStream.rdbuf();
            shaderSource = buffer.str();
            fileStream.close();
            std::cout << "content: " + shaderSource << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }

        return shaderSource;
    }

    GLuint createShaderProgram(const char *path1, const char *path2) { 
        // // glad: load all OpenGL function pointers
        // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        // {
        //     std::cout << "Failed to initialize GLAD" << std::endl;
        //     return -1;
        // }
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::cout << "0101" << std::endl;
        std::string vertShaderStr = readShaderSource(path1); 
        std::string fragShaderStr = readShaderSource(path2); 

        // // 顶点着色器，GLSL语言
        // const char *vertShaderSrc = "#version 330 core\n" 
        // "layout (location = 0) in vec3 aPos;\n"
        // "void main()\n"
        // "{\n"
        // "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" 
        // "}\0";
        // // 片元着色器
        // const char *fragShaderSrc = "#version 330 core\n"
        //     "out vec4 FragColor;\n"
        //     "void main()\n"
        //     "{\n"
        //     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        //     "}\n\0";

        const char *vertShaderSrc = vertShaderStr.c_str(); 
        const char *fragShaderSrc = fragShaderStr.c_str(); 

        glShaderSource(vertexShader, 1, &vertShaderSrc, NULL); 
        glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL); 
        std::cout << "1111" << std::endl;
        return createShaderProgram(vertexShader, fragmentShader);
    }

} // namespace OpenGLUtils

#endif // UTILS_H

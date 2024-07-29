#include <glew.h>
// #include<glad/glad.h>
#include"khrplatform.h"
#include <glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
// #include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils/OpenGLUtils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 2

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void setupVertices(void);
void init(GLFWwindow* window);
void display_static(GLFWwindow* window, double currentTime);
void display_dynamic(GLFWwindow* window, double currentTime);
void display_multi(GLFWwindow* window, double currentTime);

float cameraX, cameraY, cameraZ;

float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// 分配在 display() 函数中使用的变量空间，这样它们就不必在渲染过程中分配
GLuint mvLoc, projLoc, vLoc, mLoc;
int width, height;
float aspect;
float tf;
float tfLoc, timeFactor;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

void setupVertices(void) {    // 36个顶点，12个三角形，组成了放置在原点处的2×2×2立方体
   float vertexPositions[108] = {
      -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
   };
   glGenVertexArrays(1, vao);
   glBindVertexArray(vao[0]);
   glGenBuffers(numVBOs, vbo);

   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
   renderingProgram = OpenGLUtils::createShaderProgram(
      // "/Users/apple/Desktop/vscode/opengl/bookL/glsl/vertShader.glsl",
      "/Users/apple/Desktop/vscode/opengl/bookL/glsl/vertShader2.glsl",
      // "/Users/apple/Desktop/vscode/opengl/bookL/glsl/fragShader.glsl"
      "/Users/apple/Desktop/vscode/opengl/bookL/glsl/fragShader2.glsl"
      );
   cameraX = 0.0f; cameraY = 0.0f; cameraZ = 16.0f;
   cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // 沿Y轴下移以展示透视
   setupVertices();
}

void display_static(GLFWwindow* window, double currentTime) {
   glClear(GL_DEPTH_BUFFER_BIT);
   
   glUseProgram(renderingProgram);

   // 获取MV矩阵和投影矩阵的统一变量
   mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
   projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

   // 构建透视矩阵
   glfwGetFramebufferSize(window, &width, &height);
   aspect = (float)width / (float)height;
   pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

   // 构建视图矩阵、模型矩阵和视图-模型矩阵
   vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
   mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
   mvMat = vMat * mMat;

   // 将透视矩阵和MV矩阵复制给相应的统一变量
   glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
   glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

   // 将VBO关联给顶点着色器中相应的顶点属性
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   // 调整OpenGL设置，绘制模型
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glDrawArrays(GL_TRIANGLES, 0, 36);
}

void display_dynamic(GLFWwindow* window, double currentTime) {
   glClear(GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT);
   
   glUseProgram(renderingProgram);

   // 获取MV矩阵和投影矩阵的统一变量
   mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
   projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

   // 构建透视矩阵
   glfwGetFramebufferSize(window, &width, &height);
   aspect = (float)width / (float)height;
   pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

   // 构建视图矩阵、模型矩阵和视图-模型矩阵
   vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

   // 使用当前时间来计算x，y和z的不同变换
   tMat = glm::translate(glm::mat4(1.0f), 
      glm::vec3(sin(0.35f*currentTime)*2.0f, cos(0.52f*currentTime)*2.0f, sin(0.7f*currentTime)*2.0f));
   rMat = glm::rotate(glm::mat4(1.0f), 0.05f*(float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
   rMat = glm::rotate(rMat, 0.05f*(float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
   rMat = glm::rotate(rMat, 0.05f*(float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
   // 用1.75来调整旋转速度
   mMat = tMat * rMat;

   mvMat = vMat * mMat;

   // 将透视矩阵和MV矩阵复制给相应的统一变量
   glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
   glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

   // 将VBO关联给顶点着色器中相应的顶点属性
   glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   // 调整OpenGL设置，绘制模型
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glDrawArrays(GL_TRIANGLES, 0, 36);
}

void display_multi(GLFWwindow* window, double currentTime) {
   glClear(GL_DEPTH_BUFFER_BIT);
   glClear(GL_COLOR_BUFFER_BIT);
   
   glUseProgram(renderingProgram);

   // 获取MV矩阵和投影矩阵的统一变量
   mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
   projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
   // mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
   // vLoc = glGetUniformLocation(renderingProgram, "v_matrix");


   // 构建透视矩阵
   glfwGetFramebufferSize(window, &width, &height);
   aspect = (float)width / (float)height;
   pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

   // 构建视图矩阵、模型矩阵和视图-模型矩阵
   vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

   // 使用当前时间来计算x，y和z的不同变换
   for (int i=0; i<6; i++)    {
      tf = currentTime + i * 100;    // tf == "time factor（时间因子）"，声明为浮点类型”
      tMat = glm::translate(glm::mat4(1.0f), 
      glm::vec3(sin(0.35f*tf)*2.0f, cos(0.52f*tf)*2.0f, sin(0.7f*tf)*2.0f));
      rMat = glm::rotate(glm::mat4(1.0f), 1.75f*(float)tf, glm::vec3(0.0f, 1.0f, 0.0f));
      rMat = glm::rotate(rMat, 1.75f*(float)tf, glm::vec3(1.0f, 0.0f, 0.0f));
      rMat = glm::rotate(rMat, 1.75f*(float)tf, glm::vec3(0.0f, 0.0f, 1.0f));
      // 用1.75来调整旋转速度
      mMat = tMat * rMat;
      mvMat = vMat * mMat;

      // 将透视矩阵和MV矩阵复制给相应的统一变量
      glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

      // glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));   // 着色器需要视图矩阵的统一变量
      // timeFactor = ((float)currentTime);                             // 为了获得时间因子信息
      // tfLoc = glGetUniformLocation(renderingProgram, "tf");          // （着色器也需要它）
      // glUniform1f(tfLoc, (float)timeFactor);

      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

      // 将VBO关联给顶点着色器中相应的顶点属性
      glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);

      // 调整OpenGL设置，绘制模型
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 24);

   }
}

int main(void) {
   //     GLFWwindow* window;
   //     if (!glfwInit()) { exit(EXIT_FAILURE); }
   //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // #ifdef __APPLE__
   //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   // #endif
   //     window = glfwCreateWindow(600, 600, "LearnOpenGL", NULL, NULL);
   //     if(!window)
   //     {
   //         std::cout << "Failed to create GLFW window" << std::endl;
   //         glfwTerminate();
   //         exit(EXIT_FAILURE);
   //     }
   //     glfwMakeContextCurrent(window);
   //     if (glewInit() != GLEW_OK) 
   //     {
   //         std::cout << "Failed create GLFW window glew" << std::endl;
   //         exit(EXIT_FAILURE); 
   //     }
   //     glfwSwapInterval(1);


   GLFWwindow *window;
   if (!glfwInit()) {
      cout << "glfw init err" << endl;
      return -1;
   }
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   #ifdef __APPLE__
   cout << "I'm apple machine" << endl;
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   #endif
   unsigned int width = 800;
   unsigned int height = 600;
   window = glfwCreateWindow(width, height, "Learngl", NULL, NULL);
   if (!window) {
      cout << " can't create window!!!" << endl;
      glfwTerminate();
      return -1;
   }
   unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
   unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
   cout << "oepngl shader version: " << major << "." << minor << endl;
   glfwMakeContextCurrent(window);

   if (glewInit() != GLEW_OK) {
      cout << "Error ! " << endl;
   }
   cout << glGetString(GL_VERSION) << endl;
   std::cout << "success create GLFW window" << std::endl;
   //     // glfw: initialize and configure
   //     glfwInit();
   //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // #ifdef __APPLE__
   //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   // #endif

   //     // glfw window creation
   //     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   //     if (window == NULL)
   //     {
   //         std::cout << "Failed to create GLFW window" << std::endl;
   //         glfwTerminate();
   //         return -1;
   //     }
   //     glfwMakeContextCurrent(window);
   //     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   //    //  // glad: load all OpenGL function pointers
   //    //  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   //    //  {
   //    //      std::cout << "Failed to initialize GLAD" << std::endl;
   //    //      return -1;
   //    //  }
   init(window);

   while (!glfwWindowShouldClose(window)) {
      display_multi(window, glfwGetTime());
      // display_dynamic(window, glfwGetTime());
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}

//调整窗口大小回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
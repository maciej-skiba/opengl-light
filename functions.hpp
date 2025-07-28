#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include "shader.hpp"
#include "camera.hpp"

int InitializeOpenGL(GLFWwindow*& window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow *window, Camera *camera, float deltaTime);
void CreateLightVao(unsigned int &VAO, float* boxVertices, int numOfVertices);
void CreateBoxVao(unsigned int &VAO, float* boxVertices, int numOfVertices);
std::string LoadShader(const std::string& shaderPath);
void CheckShaderCompilation(unsigned int vertexShader);
void CheckShaderLink(unsigned int shaderProgram);
void PrintArray(float* arr, int size);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "functions.hpp"
#include "vertices.hpp" 
#include "camera.hpp"

extern float boxVertices[];
const glm::mat4 identityMatrix = glm::mat4(1.0f);

int main(void)
{
    GLFWwindow* window;
    int initSuccess = 1;

    if (InitializeOpenGL(window) != initSuccess)
    {
        return -1;
    }

    unsigned int boxVAO, lightVAO;
    
    const char* boxVertexShaderPath = "../shaders/3.3.box_vert.shad";
    const char* boxFragmentShaderPath = "../shaders/3.3.box_frag.shad";

    const char* lightVertexShaderPath = "../shaders/3.3.light_vert.shad";
    const char* lightFragmentShaderPath = "../shaders/3.3.light_frag.shad";

    Shader boxShader(boxVertexShaderPath, boxFragmentShaderPath);
    Shader lightShader(lightVertexShaderPath, lightFragmentShaderPath);

    int numOfVerticesInBox = 36;
    int numOfBoxes = 4;

    glm::vec3 boxPosition = glm::vec3( 3.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3( -3.0f, 0.0f, 0.0f);

    int boxAttributePtrIndex = 0;
    CreateBoxVao(boxVAO, boxVertices, numOfVerticesInBox, boxAttributePtrIndex);

    int lightAttributePtrIndex = 1;
    CreateBoxVao(lightVAO, boxVertices, numOfVerticesInBox, lightAttributePtrIndex);
    glEnable(GL_DEPTH_TEST);

    std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>(
        glm::vec3(0.0f, 0.0f,  10.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    glfwSetWindowUserPointer(window, mainCamera.get());

    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    float nearClippingPlane = 0.1f;
    float farClippingPlane = 100.0f;

    glm::mat4 boxModelMatrix = identityMatrix;
    glm::mat4 lightModelMatrix = identityMatrix;

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(lightVAO);

        lightModelMatrix = glm::translate(identityMatrix, lightPosition); 

        glm::mat4 projectionMatrix = 
            glm::perspective(
                glm::radians(mainCamera->Zoom),
                aspectRatio, 
                nearClippingPlane, 
                farClippingPlane);

        lightShader.UseProgram();

        unsigned int modelLocation = glGetUniformLocation(lightShader.ID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lightModelMatrix));
        unsigned int viewLocation = glGetUniformLocation(lightShader.ID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(mainCamera->GetViewMatrix()));
        unsigned int projectionLocation = glGetUniformLocation(lightShader.ID, "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        unsigned int lightColorLocation = glGetUniformLocation(lightShader.ID, "lightColor");
        glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
        
        glDrawArrays(GL_TRIANGLES, 0, numOfVerticesInBox);

        glBindVertexArray(boxVAO);

        boxModelMatrix = glm::translate(identityMatrix, boxPosition); 

        boxShader.UseProgram();

        modelLocation = glGetUniformLocation(boxShader.ID, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(boxModelMatrix));
        viewLocation = glGetUniformLocation(boxShader.ID, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(mainCamera->GetViewMatrix()));
        projectionLocation = glGetUniformLocation(boxShader.ID, "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        lightColorLocation = glGetUniformLocation(boxShader.ID, "lightColor");
        glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
        
        glDrawArrays(GL_TRIANGLES, 0, numOfVerticesInBox);

        glfwSwapBuffers(window);
        ProcessInput(window, mainCamera.get(), deltaTime);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
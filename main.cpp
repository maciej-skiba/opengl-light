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

    glm::vec3 boxPosition = glm::vec3( 0.0f, 0.0f, 0.0f);
    glm::vec3 lightPosition = glm::vec3(2.2f, 2.0f, 4.0f);

    CreateLightVao(lightVAO, boxVertices, sizeof(boxVertices));
    CreateBoxVao(boxVAO, boxVertices, sizeof(boxVertices));
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
    glm::mat4 lightModelMatrix = glm::translate(identityMatrix, lightPosition); 

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(lightVAO);


        glm::mat4 projectionMatrix = 
            glm::perspective(
                glm::radians(mainCamera->Zoom),
                aspectRatio, 
                nearClippingPlane, 
                farClippingPlane);

        lightShader.UseProgram();
        
        lightShader.SetUniformMat4("model", lightModelMatrix);
        lightShader.SetUniformMat4("view", mainCamera->GetViewMatrix());
        lightShader.SetUniformMat4("projection", projectionMatrix);
        lightShader.SetUniformVec3("lightColor", lightColor);
        
        glDrawArrays(GL_TRIANGLES, 0, numOfVerticesInBox);

        glBindVertexArray(boxVAO);

        boxModelMatrix = glm::translate(identityMatrix, boxPosition);
        boxModelMatrix = 
            glm::rotate(
                boxModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        boxShader.UseProgram();

        boxShader.SetUniformMat4("model", boxModelMatrix);
        boxShader.SetUniformMat4("view", mainCamera->GetViewMatrix());
        boxShader.SetUniformMat4("projection", projectionMatrix);
        boxShader.SetUniformVec3("cameraPos", mainCamera->Position);
        boxShader.SetUniformVec3("light.position", lightPosition);
        boxShader.SetUniformVec3("light.ambient", lightColor * 0.1f);
        boxShader.SetUniformVec3("light.diffuse", lightColor);
        boxShader.SetUniformVec3("light.specular", lightColor);
        boxShader.SetUniformVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        boxShader.SetUniformVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        boxShader.SetUniformVec3("material.specular", glm::vec3(lightColor * 0.5f));
        boxShader.SetUniformFloat("material.shininess", 32.0f);
        
        glDrawArrays(GL_TRIANGLES, 0, numOfVerticesInBox);

        glfwSwapBuffers(window);
        ProcessInput(window, mainCamera.get(), deltaTime);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
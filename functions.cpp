#include "functions.hpp"

int numOfDimensionsInVertex = 3;
int amountOfVerticesInTriangle = 3;
int amountOfCircleCenterVertices = 1;

bool firstMouseCallback = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;

int InitializeOpenGL(GLFWwindow*& window)
{
        /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "OpenGL Camera", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "cant load glew" << std::endl;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetScrollCallback(window, scroll_callback); 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ProcessInput(
    GLFWwindow *window, 
    Camera *camera,
    float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::FORWARD,  deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::BACKWARD,  deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::LEFT,  deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::RIGHT,  deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::DOWN,  deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->ProcessKeyboard(Camera_Movement::UP,  deltaTime); 

}

void CreateLightVao(
    unsigned int &VAO, 
    float* boxVertices,
    int numOfVertices)
{   
    //Vertex Array Object
    glGenVertexArrays(1, &VAO); // tworzy VAO
    glBindVertexArray(VAO);   //aktywacja tego VAO
    
    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO); //tworzy pusty VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //ustaw VBO jako aktywny array buffer; wszelkie rzeczy na GL_ARRAY_BUFFER będą dotyczyły obiektu VBO
    glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(float), boxVertices, GL_STATIC_DRAW); //wypełnij bufor (GPU) danymi

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CreateBoxVao(
    unsigned int &VAO, 
    float* boxVertices,
    int numOfVertices)
{   
    //Vertex Array Object
    glGenVertexArrays(1, &VAO); // tworzy VAO
    glBindVertexArray(VAO);   //aktywacja tego VAO
    
    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO); //tworzy pusty VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //ustaw VBO jako aktywny array buffer; wszelkie rzeczy na GL_ARRAY_BUFFER będą dotyczyły obiektu VBO
    glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(float), boxVertices, GL_STATIC_DRAW); //wypełnij bufor (GPU) danymi

    glVertexAttribPointer(
        0,  // index = location w shaderze (0 dla aPos)
        3,                      // ile wartości (vec3 → 3)
        GL_FLOAT,               // typ danych
        GL_FALSE,               // normalizacja
        8 * sizeof(float),      // odstęp między kolejnymi atrybutami (stride)
        (void*)0                // offset od początku
    );
    glEnableVertexAttribArray(0); //wybierz layout 0 jako aktywny

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!camera) return;

    if (firstMouseCallback)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouseCallback = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->ProcessMouseScroll(yoffset);
}

unsigned int LoadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
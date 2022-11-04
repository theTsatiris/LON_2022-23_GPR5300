#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>

#include "Shader.h"

using namespace std;

//Callbacks and other function
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processKeyboardInput(GLFWwindow* window);

//Settings
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 1024;
//--------

//Global utility variables
glm::vec3 displacement = glm::vec3(0.0f, 0.0f, -2.0f);

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

float movementSpeed = 1.0f;
float rotationAngle = 0.0f;
//------------------------

int main()
{
    //Initialise and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //-----------------------------

    //GLFW Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GPR5300 2022 - Graphics Lab 4", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window!!!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //Register callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    //--------------------

    //Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialise GLAD and load OpenGL function pointers!!!" << endl;
        glfwTerminate();
        return -1;
    }
    //----------------------------------------

    //Shader loading and compiling
    
    Shader mySimpleShader("shaders/myFirstShader.vs", "shaders/myFirstShader.fs");

    //----------------------------

    //Geometry loading
    float vertices[] = {
        //positions         //vertex colours    //texture coordinates
       -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    };


    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //vertex attribute which initialises the position variable in the vertex shader (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //both first arguments (0's here) are the location values for the corresponding vertex shader input

    //vertex attribute which initialises the colour variable in the vertex shader (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //vertex attribute which initialises the texCoord variable in the vertex shader (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //----------------

    //Texture loading
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //Texture wrapping configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Texture filtering configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::string imagePath = "textures/RTS_Crate.jpg";

    int width, height, numOfChannels;
    unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &numOfChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        cout << "FAILED TO LOAD TEXTURE FILE " << imagePath << "!!!" << endl;
    }
    stbi_image_free(data);

    //---------------

    //MAIN RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        //deltaTime calculation
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        //cout << "DeltaTime = " << deltaTime << endl;
        lastFrameTime = currentFrameTime;

        //Handling keyboard input
        processKeyboardInput(window);
        //-----------------------

        //Initialising buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //--------------------

        //Geometry rendering
        mySimpleShader.use(); //Tell opengl which shader to use for rendering [HOW TO DRAW]

        //Initialise texture sampler uniform
        mySimpleShader.setInt("textureObject", 0);

        //mySimpleShader.setVec3("displacement", displacement_x, displacement_y, 0.0);
        
        //Orbitting functionality
        //glUniform3f(dispVarLocation, cos(glfwGetTime()), sin(glfwGetTime()), 0.0);

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = glm::mat4(1.0); //TODO
        glm::mat4 projection = glm::mat4(1.0);

        //Simple model matrix which performs a translation and a rotation
        model = glm::translate(model, displacement);
        model = glm::rotate(model, rotationAngle, glm::vec3(0.0, 1.0, 0.0));

        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

        /*glm::vec3 color = glm::vec3(cos(glfwGetTime()), sin(glfwGetTime()), 0.5);
        mySimpleShader.setVec3("inputColor", color);*/

        mySimpleShader.setMat4("model", model);
        mySimpleShader.setMat4("view", view);
        mySimpleShader.setMat4("projection", projection);

        glBindVertexArray(VAO); //[WHAT TO DRAW]
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //------------------

        //Buffer swapping
        glfwSwapBuffers(window);
        //---------------

        //Polling for events (key pressed/released, mouse, etc.)
        glfwPollEvents();
        //------------------
    }
    //-------------------

    //Releasing structures and memory
    // TODO release allocated buffers
    glfwTerminate();
    //-------------------------------

    return 0;
}

void processKeyboardInput(GLFWwindow* window)
{
    //If ESC is pressed, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        displacement.y += movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        displacement.y -= movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        displacement.x += movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        displacement.x -= movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        displacement.z += movementSpeed * deltaTime;
        //cout << "Triangle Z = " << displacement.z << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        displacement.z -= movementSpeed * deltaTime;
        //cout << "Triangle Z = " << displacement.z << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationAngle -= movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationAngle += movementSpeed * deltaTime;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Update OpenGL viewport (i.e. the frame buffer) to match the dimensions of the GLFW window
    //after resizing.
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

//Callbacks and other function
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processKeyboardInput(GLFWwindow* window);

//Settings
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 1024;
//--------

int main()
{
    //Initialise and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //-----------------------------

    //GLFW Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GPR5300 2022 - Intro to OpenGL", NULL, NULL);
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
    // TODO
    //----------------------------

    //Geometry loading
    // TODO
    //----------------

    //Texture loading
    // TODO
    //---------------

    //MAIN RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        //Handling keyboard input
        processKeyboardInput(window);
        //-----------------------

        //Initialising buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //--------------------

        //Geometry rendering
        // TODO
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
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Update OpenGL viewport (i.e. the frame buffer) to match the dimensions of the GLFW window
    //after resizing.
    glViewport(0, 0, width, height);
}
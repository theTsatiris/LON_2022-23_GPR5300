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

//Shader Code
const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"out vec4 vertexColor;\n"
"uniform vec3 displacement;\n"
"void main()\n"
"{\n"
"	vec3 finalPosition = position + displacement;\n"
"	gl_Position = vec4(finalPosition, 1.0);\n"
"	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\0";
//-----------

//Global utility variables
float displacement_x = 0.0f;
float displacement_y = 0.0f;
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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GPR5300 2022 - Intro to Shaders", NULL, NULL);
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
    
    //Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Checking for shader compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "COMPILATION ERROR --- VERTEX SHADER" << endl << infoLog << endl;
    }
    //-------------
    
    //Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Checking for shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "COMPILATION ERROR --- FRAGMENT SHADER" << endl << infoLog << endl;
    }
    //---------------

    //Linking shaders into a program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Checking for program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "LINKING ERROR --- SHADER PROGRAM" << endl << infoLog << endl;
    }
    //------------------------------

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //----------------------------

    //Geometry loading
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //both first arguments (0's here) are the location values for the corresponding vertex shader input

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
        glUseProgram(shaderProgram); //Tell opengl which shader to use for rendering [HOW TO DRAW]

        int dispVarLocation = glGetUniformLocation(shaderProgram, "displacement");
        glUniform3f(dispVarLocation, displacement_x, displacement_y, 0.0);
        //Orbitting functionality
        //glUniform3f(dispVarLocation, cos(glfwGetTime()), sin(glfwGetTime()), 0.0);

        glBindVertexArray(VAO); //[WHAT TO DRAW]
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
        displacement_y += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        displacement_y -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        displacement_x += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        displacement_x -= 0.01f;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Update OpenGL viewport (i.e. the frame buffer) to match the dimensions of the GLFW window
    //after resizing.
    glViewport(0, 0, width, height);
}
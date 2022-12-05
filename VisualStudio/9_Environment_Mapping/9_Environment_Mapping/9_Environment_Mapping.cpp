#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Camera.h"

using namespace std;

//Callbacks
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollingCallback(GLFWwindow* window, double xOffset, double yOffset);
//----------------------------

//Other functions
void processKeyboardInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, bool flipVertically);
unsigned int loadCubeMap(vector<string> faces);
//---------------

//Settings
unsigned int SCREEN_WIDTH = 1280;
unsigned int SCREEN_HEIGHT = 1024;
//--------

//Global utility variables
glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 2.0f);

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
int frameCounter = 0;

float movementSpeed = 1.0f;
float rotationAngle = 0.0f;

float lastX = 0.0f;
float lastY = 0.0f;
bool isFirstFrame = true;

float shininess = 32.0f;
bool useBlinnPhong = true;
bool useMaterial = false;
//------------------------

//Camera declaration
Camera myCamera(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, -1.0f));
//------------------

int main()
{
    //Initialise and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //-----------------------------

    //GLFW Window creation
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GPR5300 2022 - Graphics Lab 9", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window!!!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    //Register callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollingCallback);
    //-------------------

    //Cursor grabbing setting
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //-----------------------

    //Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialise GLAD and load OpenGL function pointers!!!" << endl;
        glfwTerminate();
        return -1;
    }
    //----------------------------------------

    //Enabling-Disabling depth testing
    glEnable(GL_DEPTH_TEST);
    //--------------------------------

    //Shader loading and compiling
    Shader mySimpleShader("shaders/myFirstShader.vs", "shaders/lightMapsShader.fs");
    Shader lightBulbShader("shaders/myFirstShader.vs", "shaders/lightBulbShader.fs");
    Shader skyboxShader("shaders/skyboxShader.vs", "shaders/skyboxShader.fs");
    //----------------------------

    //Geometry loading
    float vertices[] = {
		// positions         // normals           // texture
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
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

    //vertex attribute which initialises the normal variable in the vertex shader (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //vertex attribute which initialises the texture coordinate variable in the vertex shader (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Skybox VBO and VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //----------------

    //Texture loading
    unsigned int diffuseMap = loadTexture("textures/diffuseMap.png", true);
    unsigned int specularMap = loadTexture("textures/specularMap2.png", true);

    //Cubemap loading
    vector<string> cubeFaces
    {
        "textures/skybox_2/0_right.png",
        "textures/skybox_2/1_left.png",
        "textures/skybox_2/2_top.png",
        "textures/skybox_2/3_bottom.png",
        "textures/skybox_2/4_front.png",
        "textures/skybox_2/5_back.png"
    };

    unsigned int cubeMapTexture = loadCubeMap(cubeFaces);
    //---------------

    //MAIN RENDERING LOOP
    while (!glfwWindowShouldClose(window))
    {
        //deltaTime calculation
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        //cout << "DeltaTime = " << deltaTime << endl;
        lastFrameTime = currentFrameTime;

        if(frameCounter < 2147483647)
            frameCounter++;

        //Handling keyboard input
        processKeyboardInput(window);
        //-----------------------

        //Initialising buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //--------------------

        ////**NON OPTIMAL** Skybox rendering routine
        //glDepthMask(GL_FALSE); //Disables depth testing (skybox will fail all subsequent depth tests)

        //skyboxShader.use();
        //skyboxShader.setInt("skybox", 0);

        //skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        //skyboxShader.setMat4("projection", projection);

        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        //glBindVertexArray(skyboxVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //glDepthMask(GL_TRUE); //Reenables depth testing (the rest of the geometry is rendered normally)
        ////----------------------------------------

        //Light color and position
        glm::vec3 lightPosition = glm::vec3(2.0 * glm::cos(glfwGetTime()), 2.0 * glm::sin(glfwGetTime()), 2.0);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        //------------------------

        //Geometry rendering
        mySimpleShader.use(); //Tell opengl which shader to use for rendering [HOW TO DRAW]

        ////Initialise texture sampler uniform
        //mySimpleShader.setInt("textureObject1", 0);
        //mySimpleShader.setInt("textureObject2", 1);

        //Set material properties
        mySimpleShader.setBool("useMaterial", useMaterial);
        if(useMaterial)
        { 
            mySimpleShader.setVec3("surfaceMaterial.color", glm::vec3(1.0f));
            mySimpleShader.setVec3("surfaceMaterial.ambient", glm::vec3(0.0215, 0.1745, 0.0215));
            mySimpleShader.setVec3("surfaceMaterial.diffuse", glm::vec3(0.07568, 0.61424, 0.07568));
            mySimpleShader.setVec3("surfaceMaterial.specular", glm::vec3(0.633, 0.727811 ,0.633));
            mySimpleShader.setFloat("surfaceMaterial.shininess", 0.6 * 128);
        }
        else//use light maps
        {
            mySimpleShader.setVec3("mapMaterial.ambient", glm::vec3(0.2f));
            mySimpleShader.setInt("mapMaterial.diffuse", 0);
            mySimpleShader.setInt("mapMaterial.specular", 1);
            mySimpleShader.setFloat("mapMaterial.shininess", shininess);
        }

        //Toggle Blinn-Phong
        mySimpleShader.setBool("useBlinnPhong", useBlinnPhong);

        //Initialise Phong model parameters
        mySimpleShader.setVec3("lightPosition", lightPosition);
        mySimpleShader.setVec3("lightColor", lightColor);
        mySimpleShader.setVec3("viewPosition", myCamera.position);

        //Texture binding
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        //---------------

        glm::mat4 model = glm::mat4(1.0);
        
        /*glm::vec3 color = glm::vec3(cos(glfwGetTime()), sin(glfwGetTime()), 0.5);
        mySimpleShader.setVec3("inputColor", color);*/

        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 projection = glm::mat4(1.0);

        view = myCamera.GetViewMatrix();
        projection = glm::perspective(glm::radians(myCamera.fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

        mySimpleShader.setMat4("view", view);
        mySimpleShader.setMat4("projection", projection);

        glBindVertexArray(VAO); //[WHAT TO DRAW]

        for (int i = 0; i < 10; i++)
        {
            //differently calculated model matrix per cube
            model = glm::mat4(1.0); //clear the model transform for evert cube
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i) + rotationAngle, glm::vec3(0.3f, 0.7f, 0.5f));
            //model = glm::scale(model, glm::vec3(0.2 * i + 0.2, 0.2 * i + 0.2, 0.2 * i + 0.2));
            mySimpleShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //------------------

        lightBulbShader.use();

        lightBulbShader.setMat4("view", view);
        lightBulbShader.setMat4("projection", projection);

        model = glm::mat4(1.0);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f));

        lightBulbShader.setMat4("model", model);

        lightBulbShader.setVec3("lightColor", lightColor);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        //**OPTIMAL** Skybox rendering routine
        glDepthFunc(GL_LEQUAL); //Skybox pixels pass the depth test because their depth value (1)
                                //is "lower than or equal" to the depth value of the "empty" areas
                                //of the depth buffer (where no geometry has been rendered)

        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);

        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS); //Reset to default depth testing strategy
        //----------------------------------------

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
        myCamera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        myCamera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        myCamera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        myCamera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        myCamera.ProcessKeyboard(UP, deltaTime);
     
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        myCamera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotationAngle -= movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotationAngle += movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        shininess += 100.0 * deltaTime;
        cout << shininess << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        shininess -= 100.0 * deltaTime;
        cout << shininess << endl;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (frameCounter >= 1/deltaTime)
        {
            useBlinnPhong = !useBlinnPhong;
            frameCounter = 0;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if (frameCounter >= 1 / deltaTime)
        {
            useMaterial = !useMaterial;
            frameCounter = 0;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (isFirstFrame)
    {
        lastX = xPos;
        lastY = yPos;
        isFirstFrame = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;

    lastX = xPos;
    lastY = yPos;

    myCamera.ProcessMouseInput(xOffset, yOffset, deltaTime);
}

void scrollingCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    myCamera.ProcessMouseScroll((float)yOffset);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Update OpenGL viewport (i.e. the frame buffer) to match the dimensions of the GLFW window
    //after resizing.
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
}

unsigned int loadTexture(const char* path, bool flipVertically)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //Texture wrapping configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Texture filtering configuration
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flipVertically);

    int width, height, numOfChannels;
    unsigned char* data = stbi_load(path, &width, &height, &numOfChannels, 0);
    if (data)
    {
        GLenum format;

        if (numOfChannels == 1)
        {
            format = GL_RED;
        }
        else if (numOfChannels == 3)
        {
            format = GL_RGB;
        }
        else if (numOfChannels == 4)
        {
            format = GL_RGBA;
        }
        else
        {
            cout << "TEXTURE FILE FAILED TO LOAD: INVALID CHANNEL NUMBER!!!" << endl;
            stbi_image_free(data);
            return textureID;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "FAILED TO LOAD TEXTURE FILE " << path << "!!!" << endl;
    }
    stbi_image_free(data);

    return textureID;
}

//Face order needs to be the following:
// 0. Right face (+X)
// 1. Left face (-X)
// 2. Top face (+Y)
// 3. Bottom face (-Y)
// 4. Front face (+Z)
// 5. Back face (-Z)
unsigned int loadCubeMap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    //Texture wrapping configuration
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //Texture filtering configuration
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numOfChannels;

    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &numOfChannels, 0);
        
        if (data)
        {
            GLenum format;

            if (numOfChannels == 1)
            {
                format = GL_RED;
            }
            else if (numOfChannels == 3)
            {
                format = GL_RGB;
            }
            else if (numOfChannels == 4)
            {
                format = GL_RGBA;
            }
            else
            {
                cout << "TEXTURE FILE " << faces[i] << " FAILED TO LOAD: INVALID CHANNEL NUMBER!!!" << endl;
                continue;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            cout << "FAILED TO LOAD TEXTURE FILE " << faces[i] << "!!!" << endl;
        }

        stbi_image_free(data);
    }

    return textureID;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <iostream>

#include "Shader.h"

#include "Keyboard.h"
#include "Mouse.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixVal = 0.5f;


unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z, rotate;

std::string loadShaderSrc(const char* filename);
int main(void)
{
    /*glm test
    glm::vec4 vec(1.0f, 1.0f, 1.0f, 1.0f);
    glm::mat4 transf = glm::mat4(1.0f);
    transf = glm::translate(transf, glm::vec3(5.0f, 5, 5.0f)); //translation
    transf = glm::rotate(transf, glm::radians(1.0f), glm::vec3(0.0, 0.0, 1.0)); //rotation
    transf = glm::scale(transf, glm::vec3(1.5, 1.5, 1.5)); //scaling
    vec = transf * vec;
    std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl;
    */

    glfwSetErrorCallback(error_callback);
    // Initialize GLFW
    int success;
    char infoLog[512];
    glfwInit();

    //openGL version 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPENGL Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Could not create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL libraries" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);

    glfwSetKeyCallback(window, Keyboard::keyCallBack);
    
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/glsl/vertex_core.glsl", "shaders/glsl/fragment_core.glsl");


    // vertices 
    float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // Buffers //
    // VAO, VBO
    unsigned int VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
   // textures 


    // ## textures 


    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("grass.png", &width, &height, &nChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    */
    data = stbi_load("cobblestone.png", &width, &height, &nChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    



    float timeValue = glfwGetTime();

    shader.activate();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    x = 0.0f;
    y = 0.0f;
    z = 3.0f;

    // ### main loop
    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);
       
        //render
        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        // texture 1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // texture 2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

    
        glBindVertexArray(VAO);
        // draw shapes
       
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model,  (float)glfwGetTime() * rotate, glm::vec3(0.5f));
        view = glm::translate(view, glm::vec3(-x, -y, -z));
        projection = glm::perspective(glm::radians(50.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.activate();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("mixVal", mixVal);
        
        

        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glBindVertexArray(0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}
void processInput(GLFWwindow* window)
{
    if (Keyboard::key(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
   
    // change mix value
    if (Keyboard::key(GLFW_KEY_UP))
    {
       
        mixVal += .01f;
        if (mixVal > 1)
        {
            mixVal = 1.0f;
        }
        
    }
    if (Keyboard::key(GLFW_KEY_DOWN))
    {
        mixVal -= .05f;
        if (mixVal < 0)
        {
            mixVal = 0.0f;
        }
    }
    if (Keyboard::key(GLFW_KEY_W))
    {
        z -= 0.001f;
        rotate -= 0.1f;
    }
    if (Keyboard::key(GLFW_KEY_A))
    {   
        x += 0.001;
        rotate = 0.1f;
    }
    if (Keyboard::key(GLFW_KEY_S))
    {
        z += 0.001f;
    }
    if (Keyboard::key(GLFW_KEY_D))
    {
        x -= 0.001f;
        rotate -= 0.1f;
       
    }
 
   
}

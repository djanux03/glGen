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
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
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
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL Window", NULL, NULL);
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
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback);
    /// <summary>


    Shader shader("/home/djanux/dev/cpp/glGen/shaders/vertex_core.glsl", "/home/djanux/dev/cpp/glGen/shaders/fragment_core.glsl");
    
    
    // vertices 
    float vertices[] = 
    {
        // rectangle            colors              texture coordinates
       -0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.5f,   0.0f, 0.0f,// bottom left
       -0.5f,  0.5f, 0.0f,      0.9f, 0.6f, 0.75f,  0.0f, 1.0f,// top left
        0.5f, -0.5f, 0.0f,      0.2f, 1.0f, 0.8f,   1.0f, 0.0f,// bottom right
        0.5f, 0.5f, 0.0f,      1.0f, 0.7f, 1.0f,     1.0f, 1.0f// top right
       
    };
    unsigned int indices[] = 
    {
        0, 1, 2, // first triangle
        3, 1, 2 // second triangle 
    };
    // Buffers //
    // VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);
    
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // set attribute pointer

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    

    /*
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.activate();
    shader.setMat4("transform", trans);
    glm::mat4 trans2 = glm::mat4(1.0f);
    //trans2 = glm::scale(trans2, glm::vec3(2.5f));
    trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader2.activate();
    shader2.setMat4("transform", trans);
    */
    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // textures
   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("/home/djanux/dev/cpp/glGen/assets/block.png", &width, &height, &nChannels, 0);

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

    shader.activate();
    shader.setInt("texture1", 0);
    

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        //process input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // shader.activate();
        // shader.setMat4("transform", trans);

        // draw shapes
        glBindVertexArray(VAO);
        shader.activate();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //shader2.activate();
        //shader2.setMat4("transform", trans2);

        //shader2.activate();
       // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
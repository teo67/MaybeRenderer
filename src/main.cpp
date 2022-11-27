#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
using namespace std;
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void handleErrors(GLFWwindow *window) {
    GLenum err = glGetError();
    if(err != GL_NO_ERROR) {
        string message = "undefined";
        switch(err) {
            case GL_INVALID_ENUM:
                message = "invalid enum";
                break;
            case GL_INVALID_VALUE:
                message = "invalid value";
                break;
            case GL_INVALID_OPERATION:
                message = "invalid operation";
                break;
            case GL_STACK_OVERFLOW:
                message = "STACK OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                message = "stack underflow";
                break;
            case GL_OUT_OF_MEMORY:
                message = "OUT OF MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                message = "invalid framebuffer operation";
                break;
            case GL_CONTEXT_LOST:
                message = "lost context";
                break;
        }
        cout << "Error while running: " << message << endl;
        glfwSetWindowShouldClose(window, true);
    }
}
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL); // Windowed
    if(window == NULL) {
        cout << "Failed to create window!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD!" << endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    Shader* basicShader = new Shader("shaders/colorTransformVertexShader.glsl", "shaders/colorFragmentShader.glsl");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, // back face
        0, 2, 3, 

        0, 1, 6, // left face
        0, 6, 7,

        1, 2, 5, // top face
        1, 5, 6,

        2, 3, 5, // right face
        3, 4, 5,

        4, 5, 6, // front face
        4, 6, 7, 

        0, 3, 4, // bottom face
        0, 4, 7
    }; 
    glEnable(GL_DEPTH_TEST); 
    GLuint EBO;
    GLuint VAO;
    GLuint VBO;
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);




    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        handleErrors(window);
        // rendering here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.707, 0.0, 0.707));
        basicShader->use();
        basicShader->transform(model, "model");
        basicShader->transform(view, "view");
        basicShader->transform(projection, "projection");
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // end rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    (*basicShader).end();
    glfwTerminate();
    return 0;
}
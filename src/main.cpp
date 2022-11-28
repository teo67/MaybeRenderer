#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mouse.h"
#include "Camera.h"
using namespace std;

Mouse* mouse = new Mouse();
Camera* camera = new Camera();

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouse->handleInput(xpos, ypos);
    camera->handleMouse(mouse);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->handleScroll(xoffset, yoffset);
}
void processInput(GLFWwindow *window, float dt) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->forward(dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->backward(dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->left(dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->right(dt);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, mouseCallback);  
    glfwSetScrollCallback(window, scrollCallback); 
    Shader* basicShader = new Shader(ColorOptions::VARIABLE, TextureOptions::SINGLE);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, -1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f,
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2, 
        0, 2, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    float timestamp = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        processInput(window, currentTime - timestamp);
        timestamp = currentTime;
        handleErrors(window);
        // rendering here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.707, 0.0, 0.707));
        glm::mat4 view = camera->getView();
        glm::mat4 projection = glm::perspective(glm::radians(camera->getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        basicShader->use();
        basicShader->transform(model, "model");
        basicShader->transform(view, "view");
        basicShader->transform(projection, "projection");
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
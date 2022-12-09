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
#include "BatchManager.h"
#include "shapes/ColorShape.h"
#include "shapes/RectangularPrism.h"
#include "shapes/Shape.h"
// #include "shapes/test/ZRectangularPrism.h"
// #include "shapes/test/ZShape.h"
// #include "shapes/test/ZColorShape.h"
#include <memory>

using namespace std;

Mouse mouse;
Camera camera;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouse.handleInput(xpos, ypos);
    camera.handleMouse(mouse);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.handleScroll(xoffset, yoffset);
}
void processInput(GLFWwindow *window, float dt) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.forward(dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.backward(dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.left(dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.right(dt);
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
    Shader containerShader(ColorOptions::VARIABLE_NO_LIGHT);
    //std::cout <<"made shader" << std::endl;
    
    




    BatchManager manager;

    ColorShape& shape1 = RectangularPrism::createColor(manager);







    glEnable(GL_DEPTH_TEST); 

    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // set the texture wrapping/filtering options (on the currently bound texture object)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // // load and generate the texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    // if (data) {
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // } else {
    //     cout << "Failed to load texture" << endl;
    // }
    // stbi_image_free(data);

    float timestamp = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        float dt = currentTime - timestamp;
        processInput(window, dt);
        timestamp = currentTime;
        handleErrors(window);
        // rendering here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view = camera.getView();
        glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);
        containerShader.use();
        containerShader.transform(projection * view, "transform");

        shape1.transform.yaw = currentTime * 20.0f;
        shape1.color.r = (sin(currentTime) + 1.0f) / 2.0f;
        shape1.color.b = (cos(currentTime) + 1.0f) / 2.0f;
        manager.updateAll();
        manager.renderAll();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    containerShader.end();
    manager.cleanup();
    return 0;
}
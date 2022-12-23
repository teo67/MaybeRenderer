#include "Game.h"
#include "BatchManager.h"
#include "ShapeManager.h"
#include "../shapes/ColorShape.h"
#include "../shapes/MulticolorShape.h"
#include "../shapes/TexturedShape.h"
#include "../shapes/MultitexturedShape.h"
#include <stb/stb_image.h>
#include <memory>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Game* _getGame(GLFWwindow* window) {
    return (static_cast<Game*>(glfwGetWindowUserPointer(window)));
}

void _resizeCallback(GLFWwindow* window, int width, int height) {
    Game* game = _getGame(window);
    game->resizeWindow(width, height);
}
void _mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = _getGame(window);
    game->mouse.handleInput(xpos, ypos);
    game->respondToMouse();
}
void _scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Game* game = _getGame(window);
    game->respondToScroll(xoffset, yoffset);
}

Game::Game(unsigned int width, unsigned int height) : batman(BatchManager()), shaman(ShapeManager()), texman(TextureManager()) {
    backgroundColor = Color();
    mouse = Mouse();
    camera = Camera();
    this->width = width;
    this->height = height;
    fov = defaultFov;
    updateProjectionMatrix();
    _dt = 0.0f;
    previousTime = 0.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL); // Windowed
    if(window == NULL) {
        std::cout << "Failed to create window!" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, _resizeCallback);
    glfwSetCursorPosCallback(window, _mouseCallback);
    glfwSetScrollCallback(window, _scrollCallback);

    glEnable(GL_DEPTH_TEST); 
    batman.init();
}

void Game::resizeWindow(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);
    updateProjectionMatrix();
}

float Game::dt() {
    return _dt;
}

float Game::time() {
    return previousTime;
}

void Game::close() {
    glfwSetWindowShouldClose(window, true);
}

bool Game::closed() {
    return glfwWindowShouldClose(window);
}

bool Game::isKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void Game::respondToMouse() {
    camera.handleMouse(mouse); // can be edited by user
}

void Game::respondToScroll(double x, double y) {
}

void Game::setFOV(float _fov) {
    fov = _fov;
    updateProjectionMatrix();
}

Game::~Game() {
    shaman.cleanup();
    batman.cleanup();
    texman.cleanup();
    glfwTerminate();
}
void Game::render() {
    float currentTime = glfwGetTime();
    _dt = currentTime - previousTime;
    previousTime = currentTime;
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    batman.updateAll();
    batman.renderAll(projectionMatrix * camera.getView());
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::updateProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
}

ColorShape& Game::generateColorShape(const VertexIndexInfo& viInfo, bool isStatic) {
    ColorShape sha(viInfo, isStatic);
    std::shared_ptr<ColorNode> test = std::make_shared<ColorNode>(ColorNode(sha));
    batman.addShape(test);
    return test->getShape();
}

MulticolorShape& Game::generateMulticolorShape(const VertexIndexInfo& viInfo, bool isStatic) {
    MulticolorShape sha(viInfo, isStatic);
    std::shared_ptr<MulticolorNode> test = std::make_shared<MulticolorNode>(MulticolorNode(sha));
    batman.addShape(test);
    return test->getShape();
}

TexturedShape& Game::generateTexturedShape(const VertexIndexInfo& viInfo, bool isStatic) {
    TexturedShape sha(viInfo, isStatic);
    std::shared_ptr<TextureNode> test = std::make_shared<TextureNode>(TextureNode(sha));
    batman.addShape(test);
    return test->getShape();
}

MultitexturedShape& Game::generateMultitexturedShape(const VertexIndexInfo& viInfo, bool isStatic) {
    MultitexturedShape sha(viInfo, isStatic);
    std::shared_ptr<MultitextureNode> test = std::make_shared<MultitextureNode>(MultitextureNode(sha));
    batman.addShape(test);
    return test->getShape();
}
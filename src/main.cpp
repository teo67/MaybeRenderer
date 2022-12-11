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
#include "shapes/MulticolorShape.h"
#include "shapes/Shape.h"
#include <memory>
#include "Game.h"
void processInput(Game& game) {
    float dt = game.dt();
    if(game.isKeyPressed(GLFW_KEY_ESCAPE))
        game.close();
    if (game.isKeyPressed(GLFW_KEY_W))
        game.camera.forward(dt);
    if (game.isKeyPressed(GLFW_KEY_S))
        game.camera.backward(dt);
    if (game.isKeyPressed(GLFW_KEY_A))
        game.camera.left(dt);
    if (game.isKeyPressed(GLFW_KEY_D))
        game.camera.right(dt);
}
int main() {
    Game game(800, 600);
    MulticolorShape& shape1 = game.generateMulticolorShape(game.shaman.pyramid(4), false);
    Color& topColor = shape1.getColor(4);
    Color& aColor = shape1.getColor(0);
    Color& bColor = shape1.getColor(1);
    Color& cColor = shape1.getColor(2);
    Color& dColor = shape1.getColor(3);
    while(!game.closed()) {
        processInput(game);
        float currentTime = game.time();
        shape1.transform.yaw = currentTime * 20.0f;
        topColor.r = (sin(currentTime) + 1.0f) / 2.0f;
        aColor.g = (cos(currentTime) + 1.0f) / 2.0f;
        aColor.b = (cos(currentTime) + 1.0f) / 2.0f;
        bColor.g = (cos(currentTime) + 1.0f) / 2.0f;
        bColor.b = (sin(currentTime) + 1.0f) / 2.0f;
        cColor.g = (sin(currentTime) + 1.0f) / 2.0f;
        cColor.b = (sin(currentTime) + 1.0f) / 2.0f;
        dColor.g = (sin(currentTime) + 1.0f) / 2.0f;
        dColor.b = (cos(currentTime) + 1.0f) / 2.0f;
        game.backgroundColor.b = (cos(currentTime) + 1.0f) / 2.0f;
        game.render();
    }
    return 0;
}
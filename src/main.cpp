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
#include "shapes/TexturedShape.h"
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
    Game game(1600, 1200);
    game.texman.registerTexture("grass", "textures/grass.jpg", false);
    game.texman.registerTexture("grassTop", "textures/grass-top.jpg", false);
    game.texman.registerTexture("grassBottom", "textures/grass-bottom.jpg", false);
    MultitexturedShape& shape = game.generateMultitexturedShape(game.shaman.prism(8, true), true);
    shape.setTexture(0, game.texman.getTexture("grassBottom"));
    shape.setTexture(1, game.texman.getTexture("grassTop"));
    for(int i = 2; i < 10; i++) {
        shape.setTexture(i, game.texman.getTexture("grass"));
    }

    game.backgroundColor.r = 0.0;
    game.backgroundColor.g = 0.0;
    game.backgroundColor.b = 1.0;
    while(!game.closed()) {
        processInput(game);
        //shape1.setPosition(sinf(game.time()), 0.0f, 0.0f);
        game.render();
    }
    return 0;
}
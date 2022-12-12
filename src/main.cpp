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
    Game game(800, 600);
    game.registerTexture("grass", "textures/grass.jpg");
    game.registerTexture("container", "textures/container.jpg");
    

    
    TexturedShape& shape1 = game.generateTexturedShape(game.shaman.prism(4), false, "container");
    TexturedShape& shape2 = game.generateTexturedShape(game.shaman.prism(4), false, "grass");
TexturedShape& shape3 = game.generateTexturedShape(game.shaman.prism(4), false, "grass");
    
    shape2.setPosition(2.0, 0.0, 0.0);
    shape3.setPosition(0.0, 0.0, 2.0);
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
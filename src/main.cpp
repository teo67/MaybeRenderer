#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shapes/ColorShape.h"
#include "shapes/MulticolorShape.h"
#include "shapes/TexturedShape.h"
#include "shapes/Shape.h"
#include "stuff/Game.h"

#include <iostream>
#include <queue>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float CUBE_UNIT = sqrtf(2.0f) / 2.0f; // width of a unit cube
const float LOOK_SPEED = 0.03f;
const float BLOCK_SIZE = 1.0f;
const unsigned int BOARD_SIZE = 10;
const float RADIUS = BLOCK_SIZE * BOARD_SIZE * CUBE_UNIT * 1.5f; // add some extra space

class MyGame : public Game {
    public:
        void respondToMouse() {
            float y = camera.position.y + mouse.getDy() * LOOK_SPEED;
            camera.position.y = y;
        }
        MyGame(unsigned int width, unsigned int height) : Game(width, height) {}
};

ColorShape* generateSnakePiece(MyGame& game, float x, float y, float z) {
    ColorShape& sha = game.generateColorShape(game.shaman.prism(4, false), false);
    sha.setColor(0.0f, 1.0f, 0.0f);
    sha.setPosition(x, y, z);
    sha.setScale(0.0f, 0.0f, 0.0f);
    return &sha;
}

struct vec3 {
    int x;
    int y; 
    int z;
    vec3(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}
    glm::vec3 multiply(float w) { // multiply by float
        return glm::vec3(x * w, y * w, z * w);
    }
    glm::vec3 add(float w) {
        return glm::vec3(x + w, y + w, z + w);
    }
    vec3 add(vec3& w) {
        return vec3(x + w.x, y + w.y, z + w.z);
    }
    vec3 multiply(vec3& w) {
        return vec3(x * w.x, y * w.y, z * w.z);
    }
    glm::vec3 add(const glm::vec3& w) {
        return glm::vec3(x + w.x, y + w.y, z + w.z);
    }
};

int main() {
    MyGame game(1600, 1200);
    std::queue<ColorShape*> queue;
    float resizeSpeed = 1.0f;
    float cameraSpeed = 10.0f;
    vec3 movement = vec3(1, 0, 0);
    glm::vec3 unit(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
    
    unsigned int initialSize = 4;
    for(int i = 0; i < initialSize; i++) {
        ColorShape* res = generateSnakePiece(game, i * CUBE_UNIT * BLOCK_SIZE, 0, 0);
        res->setScale(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        queue.push(res);
    }
    

    glm::vec3 backpos(3 * BLOCK_SIZE * CUBE_UNIT, 0, 0);
    glm::vec3 frontpos(0, 0, 0);

    ColorShape& ref = game.generateColorShape(game.shaman.prism(4, false), true);
    ref.setPosition(-5.0f, 0.0f, 0.0f);
    ref.setColor(1.0f, 0.0f, 0.0f);

    float currentSize = 0.0f;
    
    while(!game.closed()) {
        float time = game.time() * cameraSpeed;
        game.camera.position.x = RADIUS * cosf(glm::radians(time));
        game.camera.position.z = RADIUS * sinf(glm::radians(time));
        game.camera.setYaw(180.0f + time);
        float dt = game.dt();
        game.render();
        ColorShape* back = queue.back();
            ColorShape* front = queue.front();
        if(currentSize >= 1.0f) {
            if (game.isKeyPressed(GLFW_KEY_W))
                movement = vec3(0, 1, 0);
            if (game.isKeyPressed(GLFW_KEY_S))
                movement = vec3(0, -1, 0);
            if (game.isKeyPressed(GLFW_KEY_A))
                movement = vec3(-1, 0, 0);
            if (game.isKeyPressed(GLFW_KEY_D))
                movement = vec3(1, 0, 0);
            if (game.isKeyPressed(GLFW_KEY_Q)) {
                movement = vec3(0, 0, -1);
            }
            if(game.isKeyPressed(GLFW_KEY_E)) {
                movement = vec3(0, 0, 1);
            }

            back->setScale(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            back->setPosition(backpos.x, backpos.y, backpos.z);
            front->disable(true);

            queue.pop();
            frontpos = glm::vec3(queue.front()->transform.x, queue.front()->transform.y, queue.front()->transform.z);
            backpos += movement.multiply(BLOCK_SIZE * CUBE_UNIT);
            queue.push(generateSnakePiece(game, backpos.x, backpos.y, backpos.z));
            currentSize = 0.0f;
        } else {
            glm::vec3 m1 = unit - movement.multiply(BLOCK_SIZE * (1.0f - currentSize));
            glm::vec3 m2 = unit - movement.multiply(BLOCK_SIZE * currentSize);
            back->setScale(m1.x, m1.y, m1.z);
            front->setScale(m2.x, m2.y, m2.z);
            glm::vec3 nback = backpos + (movement.multiply(0.5f * (currentSize - 1.0f))) * BLOCK_SIZE * CUBE_UNIT;
            glm::vec3 nfront = frontpos + (movement.multiply(0.5f * currentSize)) * BLOCK_SIZE * CUBE_UNIT;
            back->setPosition(nback.x, nback.y, nback.z);
            front->setPosition(nfront.x, nfront.y, nfront.z);
            currentSize += dt * resizeSpeed;
        }
        if(game.isKeyPressed(GLFW_KEY_ESCAPE)) {
            game.close();
        }
    }
    return 0;
}
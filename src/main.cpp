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
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ColorSide {
    Color color;
    unsigned int face;
    ColorSide(Color _color, unsigned int _face) {
        color = _color;
        face = _face;
    }
};

const ColorSide TOP(Color(1.0f, 0.0f, 0.0f), 1); // red
const ColorSide BOTTOM(Color(1.0f, 0.647f, 0.0f), 0); // orange
const ColorSide BACK(Color(1.0f, 1.0f, 0.0f), 3); // yellow
const ColorSide LEFT(Color(0.0f, 1.0f, 0.0f), 4); // green
const ColorSide RIGHT(Color(0.0f, 0.0f, 1.0f), 2); // blue
const ColorSide FRONT(Color(1.0f, 1.0f, 1.0f), 5); // white

MulticolorShape* createBlankPiece(Game& game, const VertexIndexInfo& viInfo, int x, int y, int z, float scale) {
    float t = scale * sqrtf(2.0f) / 2.0f;
    MulticolorShape& r = game.generateMulticolorShape(viInfo, false);
    r.setScale(scale, scale, scale);
    r.setPosition(x * t, y * t, z * t);
    return &r;
}

void editFaceColor(MulticolorShape* shape, ColorSide color, const VertexIndexInfo& viInfo) {
    for(int i = 0; i < viInfo.faceNumbers.size(); i++) {
        if(viInfo.faceNumbers[i] == color.face) {
            shape->setColor(i, color.color);
        }
    }
}

MulticolorShape* createPieceWithEdits(Game& game, const VertexIndexInfo& viInfo, int x, int y, int z, float scale, std::vector<ColorSide> colors) {
    MulticolorShape* res = createBlankPiece(game, viInfo, x, y, z, scale);
    for(int i = 0; i < colors.size(); i++) {
        editFaceColor(res, colors[i], viInfo);
    }
    return res;
}

int main() {
    Game game(1600, 1200);
    const unsigned int size = 5;
    const float blockSize = 1.0f;
    const float moveSpeed = 2.0f;
    if(size < 2) {
        std::cout << "Size must be 2 or greater!" << std::endl;
        return -1;
    }
    MulticolorShape* pieces[size][size][size]; // highest order: X (columns), middle: Y (rows), lowest: Z (slices)
    const VertexIndexInfo& info = game.shaman.prism(4, true);

    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            for(int z = 0; z < size; z++) {
                std::vector<ColorSide> sides;
                if(x == 0) {
                    sides.push_back(LEFT);
                } else if(x == size - 1) {
                    sides.push_back(RIGHT);
                }
                if(y == 0) {
                    sides.push_back(BOTTOM);
                } else if(y == size - 1) {
                    sides.push_back(TOP);
                }
                if(z == 0) {
                    sides.push_back(BACK);
                } else if(z == size - 1) {
                    sides.push_back(FRONT);
                }
                pieces[x][y][z] = createPieceWithEdits(game, info, x, y, z, blockSize, sides);
            }
        }
    }

    while(!game.closed()) {
        game.render();
        if(game.isKeyPressed(GLFW_KEY_ESCAPE)) {
            game.close();
        }
        float dt = game.dt();
        if(game.isKeyPressed(GLFW_KEY_W)) {
            game.camera.forward(dt * moveSpeed);
        }
        if(game.isKeyPressed(GLFW_KEY_A)) {
            game.camera.left(dt * moveSpeed);
        }
        if(game.isKeyPressed(GLFW_KEY_S)) {
            game.camera.backward(dt * moveSpeed);
        }
        if(game.isKeyPressed(GLFW_KEY_D)) {
            game.camera.right(dt * moveSpeed);
        }
    }
    return 0;
}
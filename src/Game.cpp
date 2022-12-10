#include "Game.h"
#include "BatchManager.h"
#include "ShapeManager.h"
#include "shapes/ColorShape.h"
#include "shapes./MulticolorShape.h"
#include <memory>

Game::Game() : batman(BatchManager()), shaman(ShapeManager()) {
}
Game::~Game() {
    shaman.cleanup();
    batman.cleanup();
}
void Game::render() {
    batman.updateAll();
    batman.renderAll();
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
#include <vector>
#include "shapes/Shape.h"
#include "BatchManager.h"
#include "BatchInfo.h"
#include <iostream>
#include <memory>
#include <optional>

BatchManager::BatchManager() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        batches[i] = std::nullopt;
    }
}
void BatchManager::addShape(std::shared_ptr<Shape> shape) {
    const BatchInfo format = shape->getFormat();
    if(!batches[format.index].has_value()) {
        batches[format.index].emplace(Batch(format.sizes, format.isStatic));
    }
    batches[format.index]->addShapeToQueue(shape);
}
void BatchManager::updateAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].has_value()) {
            batches[i]->update();
        }
    }
}
void BatchManager::renderAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].has_value()) {
            batches[i]->draw();
        }
    }
}
void BatchManager::updateStatic(std::shared_ptr<Shape> shape) {
    unsigned int index = shape->getFormat().index;
    batches[index]->marked = true;
}
void BatchManager::cleanup() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].has_value()) {
            batches[i]->end();
        }
    }
}
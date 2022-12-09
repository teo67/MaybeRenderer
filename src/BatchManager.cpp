#include <vector>
#include "shapes/Shape.h"
#include "shapes/ColorShape.h"
#include "BatchManager.h"
#include "BatchInfo.h"
#include <iostream>
#include <memory>
#include <optional>

BatchManager::BatchManager() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        batches[i] = Batch();
    }
}
void BatchManager::addShape(const std::shared_ptr<Node1>& _shape) {
    //std::cout << _shape->getShape().getNumVertices() << std::endl;
    const BatchInfo format = _shape->getShape().getFormat();
    std::cout << format.index << std::endl;
    if(!batches[format.index].initialized) {
        batches[format.index].init(format.sizes, format.isStatic);
    }
    batches[format.index].addShapeToQueue(_shape);
}
void BatchManager::updateAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].initialized) {
            batches[i].update();
        }
    }
}
void BatchManager::renderAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].initialized) {
            batches[i].draw();
        }
    }
}
void BatchManager::updateStatic(Shape& shape) {
    unsigned int index = shape.getFormat().index;
    batches[index].marked = true;
}
void BatchManager::cleanup() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i].initialized) {
            batches[i].end();
        }
    }
}
#include <vector>
#include "shapes/Shape.h"
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
void BatchManager::addShape(Shape& shape) {
    const BatchInfo format = shape.getFormat();
    if(!batches[format.index].initialized) {
        batches[format.index].init(format.sizes, format.isStatic);
    }
    batches[format.index].addShapeToQueue(shape);
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
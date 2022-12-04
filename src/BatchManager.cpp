#include <vector>
#include "shapes/Shape.h"
#include "BatchManager.h"
#include "BatchInfo.h"
#include <iostream>

BatchManager::BatchManager() {
    batches = new Batch*[6] { nullptr };
}
void BatchManager::addShape(Shape* shape) {
    const BatchInfo format = shape->getFormat();
    if(batches[format.index] == nullptr) {
        batches[format.index] = new Batch(format.sizes, format.isStatic);
    }
    batches[format.index]->addShapeToQueue(shape);
}
void BatchManager::updateAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i] != nullptr) {
            batches[i]->update();
        }
    }
}
void BatchManager::renderAll() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i] != nullptr) {
            batches[i]->draw();
        }
    }
}
void BatchManager::updateStatic(Shape* shape) {
    unsigned int index = shape->getFormat().index;
    batches[index]->marked = true;
}
void BatchManager::cleanup() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        if(batches[i] != nullptr) {
            batches[i]->end();
        }
        delete batches[i];
    }
}
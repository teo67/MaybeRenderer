#include <vector>
#include "shapes/Shape.h"
#include "shapes/ColorShape.h"
#include "BatchManager.h"
#include "BatchInfo.h"
#include <iostream>
#include <memory>
#include <optional>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

BatchManager::BatchManager() {}
void BatchManager::init() { // to be run after initializing opengl
    int numTextures = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextures);
    for(int i = 0; i < NUM_BATCHES; i++) {
        batches[i] = Batch((unsigned int)numTextures);
    }
    shaders[COLOR_SHADER] = Shader(ColorOptions::VARIABLE_NO_LIGHT);
    shaders[TEXTURE_SHADER] = Shader(TextureOptions::SINGLE, numTextures);
    shaders[TEXTURE_COLOR_SHADER] = Shader(ColorOptions::VARIABLE_NO_LIGHT, TextureOptions::SINGLE, numTextures);
}
void BatchManager::addShape(const std::shared_ptr<Node1>& _shape) {
    //std::cout << _shape->getShape().getNumVertices() << std::endl;
    const BatchInfo format = _shape->getShape().getFormat();
    //std::cout << format.index << std::endl;
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
void BatchManager::renderPair(unsigned int index1, unsigned int index2, unsigned int shaderIndex, glm::mat4 transform) {
    if(batches[index1].initialized || batches[index2].initialized) {
        shaders[shaderIndex].use();
        shaders[shaderIndex].transform(transform, "transform");
        batches[index1].draw();
        batches[index2].draw();
    }
}   
void BatchManager::renderAll(glm::mat4 transform) {
    renderPair(STATIC_COLOR.index, DYNAMIC_COLOR.index, COLOR_SHADER, transform);
    renderPair(STATIC_TEXTURE.index, DYNAMIC_TEXTURE.index, TEXTURE_SHADER, transform);
    renderPair(STATIC_COLORTEXTURE.index, DYNAMIC_COLORTEXTURE.index, TEXTURE_COLOR_SHADER, transform);
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
    for(int i = 0; i < NUM_SHADERS; i++) {
        shaders[i].end();
    }
}
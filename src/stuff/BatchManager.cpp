#include <vector>
#include "../shapes/Shape.h"
#include "../shapes/ColorShape.h"
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
#include "TextureSet.h"

BatchManager::BatchManager() {
    for(int i = 0; i < NUM_BATCHES; i++) {
        batches[i] = Batch();
    }
}
void BatchManager::init() { // to be run after initializing opengl
    int numTextures = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextures);
    TextureSet::maxTextureUnits = numTextures;
    std::vector<int> vec(numTextures);
    for(int i = 0; i < numTextures; i++) {
        vec[i] = i; // generate sampler array
    }
    shaders[COLOR_SHADER] = Shader(ColorOptions::VARIABLE_NO_LIGHT);
    shaders[TEXTURE_SHADER] = Shader(TextureOptions::SINGLE, numTextures);
    shaders[TEXTURE_COLOR_SHADER] = Shader(ColorOptions::VARIABLE_NO_LIGHT, TextureOptions::SINGLE, numTextures);

    shaders[TEXTURE_SHADER].use();
    shaders[TEXTURE_SHADER].sendUniformArray(numTextures, static_cast<int*>(&vec[0]), "texture1");

    shaders[TEXTURE_COLOR_SHADER].use();
    shaders[TEXTURE_COLOR_SHADER].sendUniformArray(numTextures, static_cast<int*>(&vec[0]), "texture1");
}
void BatchManager::addShape(const std::shared_ptr<Node1>& _shape) {
    //std::cout << _shape->getShape().getNumVertices() << std::endl;
    const BatchInfo format = _shape->getShape().getFormat();
    //std::cout << format.index << std::endl;
    if(!batches[format.index].initialized) {
        //std::cout << "initializing " << format.index << std::endl;
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
    bool i1 = batches[index1].initialized;
    bool i2 = batches[index2].initialized;
    if(i1 || i2) {
        shaders[shaderIndex].use();
        shaders[shaderIndex].transform(transform, "transform");
        if(i1) {
            batches[index1].draw();
        } 
        if(i2) {
            batches[index2].draw();
        }
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
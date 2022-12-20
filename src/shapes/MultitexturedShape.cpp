#include "MultitexturedShape.h"
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include "../TextureSet.h"
MultitextureNode::MultitextureNode(MultitexturedShape& _shape) : shape(_shape) {}
MultitexturedShape& MultitextureNode::getShape() {
    return shape;
}

MultitexturedShape::MultitexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic)
: Shape(PositionInfo(), _vertexIndexInfo, _isStatic) {
    textures = std::vector<unsigned int>(_vertexIndexInfo.faceIndexOffsets.size()); // same number of textures as faces
}

void MultitexturedShape::setTexture(unsigned int index, GLuint texture) {
    if(index < 0 || index >= textures.size()) {
        std::cout << "Index " << index << " is out of range! This shape only has " << textures.size() << " faces!" << std::endl;
    } else {
        textures[index] = texture;
    }
}

const BatchInfo& MultitexturedShape::getFormat() {
    return isStatic ? STATIC_TEXTURE : DYNAMIC_TEXTURE;
}

void MultitexturedShape::appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex) {
    std::vector<GLuint> translated(textures.size());
    for(int i = 0; i < textures.size(); i++) {
        translated[i] = textureSet.getTexture(textures[i], vertexIndexInfo.faceIndexOffsets[i]);
    }
    fillPositionInfo(vertexData, index, 7);
    for(int i = 0; i < vertexIndexInfo.faceNumbers.size(); i++) {
        vertexData[index + 6 + i * 7] = translated[vertexIndexInfo.faceNumbers[i]];
    }
    for(int i = 0; i < vertexIndexInfo.texCoordinates.size(); i++) {
        vertexData[index + 4 + i * 7] = vertexIndexInfo.texCoordinates[i].x;
        vertexData[index + 5 + i * 7] = vertexIndexInfo.texCoordinates[i].y;
    }
}
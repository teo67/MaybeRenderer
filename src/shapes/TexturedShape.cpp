#include "TexturedShape.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
TexturedShape::TexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic)
: Shape(PositionInfo(), _vertexIndexInfo, _isStatic) {
    texture = 0;
    //std::cout << "setting texture to " << texture << std::endl;
}

void TexturedShape::setTexture(GLuint texture) {
    this->texture = texture;
}

void TexturedShape::appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex) {
    GLuint translatedNum = textureSet.getTexture(texture, firstIndex);
    std::cout << translatedNum << std::endl;
    fillPositionInfo(vertexData, index, 7);
    fillSameValue(vertexData, index + 6, 7, translatedNum);
    //std::cout << vertexData[index + 6] << std::endl;
    for(int i = 0; i < vertexIndexInfo.texCoordinates.size(); i++) {
        vertexData[index + 4 + i * 7] = vertexIndexInfo.texCoordinates[i].x;
        vertexData[index + 5 + i * 7] = vertexIndexInfo.texCoordinates[i].y;
    }
}

TextureNode::TextureNode(TexturedShape& _shape) : Node1(), shape(_shape) {
}

TexturedShape& TextureNode::getShape() {
    return shape;
}

const BatchInfo& TexturedShape::getFormat() {
    return isStatic ? STATIC_TEXTURE : DYNAMIC_TEXTURE;
}
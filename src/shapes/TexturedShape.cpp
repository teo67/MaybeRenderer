#include "TexturedShape.h"
#include <string>
TexturedShape::TexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic, unsigned int _texture)
: Shape(PositionInfo(), _vertexIndexInfo, _isStatic) {
    texture = _texture;
}

void TexturedShape::setTexture(unsigned int texture) {
    this->texture = texture;
}

void TexturedShape::appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex) {
    unsigned int translatedNum = textureSet.getTexture(texture, firstIndex);
    fillPositionInfo(vertexData, index, 7);
    fillSameValue(vertexData, index + 6, 7, texture);
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
#include "ColorShape.h"
#include "Shape.h"
#include "../stuff/BatchInfo.h"
#include <vector>
#include <memory>
ColorShape::ColorShape(PositionInfo _positionInfo, const VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
Shape(_positionInfo, _vertexIndexInfo, _isStatic), color(Color())
{
    
}

ColorShape::ColorShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
ColorShape(PositionInfo(), _vertexIndexInfo, _isStatic){}

void ColorShape::appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex) {
    fillPositionInfo(vertexData, index, 7);
    fillSameValue(vertexData, index + 4, 7, color.r);
    fillSameValue(vertexData, index + 5, 7, color.g);
    fillSameValue(vertexData, index + 6, 7, color.b);
}

void ColorShape::setColor(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

ColorNode::ColorNode(ColorShape& _shape) : Node1(), shape(_shape) {
}

ColorShape& ColorNode::getShape() {
    return shape;
}

Color::Color() {
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

Color::Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
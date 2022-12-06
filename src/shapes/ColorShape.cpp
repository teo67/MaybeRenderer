#include "ColorShape.h"
#include "Shape.h"
#include "../Debugger.h"
#include <vector>
ColorShape::ColorShape(PositionInfo _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
Shape(_positionInfo, _vertexIndexInfo, _isStatic), color(Color())
{
}

ColorShape::ColorShape(VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
ColorShape(PositionInfo(), _vertexIndexInfo, _isStatic){}

void ColorShape::appendVertexData(std::vector<float>& vertexData, unsigned int index) {
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
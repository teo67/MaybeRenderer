#include "ColorShape.h"
#include "Shape.h"
#include "../Debugger.h"
#include <vector>
ColorShape::ColorShape(PositionInfo& _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
Shape(_positionInfo, _vertexIndexInfo, _isStatic)
{
    r = 0.0f;
    g = 0.0f;
    b = 0.0f;
}

void ColorShape::appendVertexData(std::vector<float>& vertexData, unsigned int index) {
    fillPositionInfo(vertexData, index, 7);
    fillSameValue(vertexData, index + 4, 7, r);
    fillSameValue(vertexData, index + 5, 7, g);
    fillSameValue(vertexData, index + 6, 7, b);
}
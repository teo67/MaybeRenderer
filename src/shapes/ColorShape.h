#ifndef COLORSHAPE_H
#define COLORSHAPE_H
#include "Shape.h"
#include <vector>
class ColorShape : public Shape {
    public:
        float r;
        float g;
        float b;
        ColorShape(PositionInfo& _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index);
};
#endif
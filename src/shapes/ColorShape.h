#ifndef COLORSHAPE_H
#define COLORSHAPE_H
#include "Shape.h"
#include <vector>
struct Color {
    float r;
    float g;
    float b;
};
class ColorShape : public Shape {
    public:
        Color color;
        ColorShape(PositionInfo _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        ColorShape(VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index);
        void setColor(float r, float g, float b);
};
#endif
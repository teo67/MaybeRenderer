#ifndef COLORSHAPE_H
#define COLORSHAPE_H
#include "Shape.h"
#include <vector>
#include <memory>
struct Color {
    float r;
    float g;
    float b;
};
class ColorShape : public Shape {
    public:
        Color color;
        ColorShape(PositionInfo _positionInfo, const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        ColorShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index);
        void setColor(float r, float g, float b);
};
class ColorNode : public Node1 {
    private:
        ColorShape shape;
    public:
        ColorNode(ColorShape& _shape);
        ColorShape& getShape() override;
};
#endif
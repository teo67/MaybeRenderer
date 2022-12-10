#ifndef MULTICOLORSHAPE_H
#define MULTICOLORSHAPE_H
#include "Shape.h"
#include <vector>
#include <memory>
#include "ColorShape.h"
class MulticolorShape : public Shape {
    private:
        std::vector<Color> colors;
    public:
        MulticolorShape(PositionInfo _positionInfo, const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        MulticolorShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index);
        void setColor(unsigned int vertexNumber, float r, float g, float b);
        Color& getColor(unsigned int vertexNumber);
};
class MulticolorNode : public Node1 {
    private:
        MulticolorShape shape;
    public:
        MulticolorNode(MulticolorShape& _shape);
        MulticolorShape& getShape() override;
};
#endif
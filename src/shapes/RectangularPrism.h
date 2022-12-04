#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H
#include "Shape.h"
#include "ColorShape.h"
class RectangularPrism : public ColorShape {
    public:
        void appendVertexData(float* vertexData, unsigned int index);
        void appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstIndex);
        unsigned int getNumVertices();
        unsigned int getNumIndices();
        RectangularPrism(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
};
#endif
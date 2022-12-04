#ifndef COLORSHAPE_H
#define COLORSHAPE_H
#include "Shape.h"
class ColorShape : public Shape { 
    protected:
        void init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
    public:
        float r;
        float g;
        float b;
        ColorShape();
};
#endif
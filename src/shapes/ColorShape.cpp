#include "ColorShape.h"
void ColorShape::init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale) {
    Shape::init(isStatic, x, y, z, pitch, yaw, roll, xScale, yScale, zScale);
    r = 0;
    g = 0;
    b = 0;
}
ColorShape::ColorShape() {}
#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H
#include "Shape.h"
#include "ColorShape.h"

namespace RectangularPrism {
    ColorShape createColor(PositionInfo positionInfo, bool isStatic);
    ColorShape createColor(bool isStatic);
    ColorShape createColor();
}

#endif
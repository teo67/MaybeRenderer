#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H
#include "Shape.h"
#include "ColorShape.h"
#include "../BatchManager.h"

namespace RectangularPrism {
    ColorShape& createColor(BatchManager& batman, PositionInfo& positionInfo, bool isStatic);
    ColorShape& createColor(BatchManager& batman, bool isStatic);
    ColorShape& createColor(BatchManager& batman);
    ColorShape& create(BatchManager& batman, VertexIndexInfo& viInfo, PositionInfo& info, bool isStatic);
    void test(std::shared_ptr<Node1>& in);
}

#endif
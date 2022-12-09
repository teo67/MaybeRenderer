#include "shapes/Shape.h"
#include "shapes/ColorShape.h"
#include "BatchManager.h"
#include <map>
class ShapeManager {
    private:
        BatchManager& batman;
        std::map<unsigned int, VertexIndexInfo> prisms;
        VertexIndexInfo& generatePrism(unsigned int numSides);
        std::map<unsigned int, VertexIndexInfo>::const_iterator& getFromMap(std::map<unsigned int, VertexIndexInfo>& map);
    public:
        ColorShape& generateColorShape(VertexIndexInfo& viInfo, PositionInfo& info, bool isStatic);
        VertexIndexInfo prism(unsigned int numSides);
        ShapeManager(BatchManager& _batman);
};
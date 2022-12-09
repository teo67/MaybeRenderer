#include "ShapeManager.h"
#include "BatchManager.h"
#include "shapes/Shape.h"
#include "shapes/ColorShape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

ShapeManager::ShapeManager(BatchManager& _batman) : batman(_batman) {
    prisms = {};
}

VertexIndexInfo& ShapeManager::generatePrism(unsigned int numSides) {

}

VertexIndexInfo ShapeManager::prism(unsigned int numSides) {
    std::map<unsigned int, VertexIndexInfo>::const_iterator& found = getFromMap(prisms);
    if(found == prisms.end()) {
        return generatePrism(numSides);
    }
    return found->second;
}

ColorShape& ShapeManager::generateColorShape(VertexIndexInfo& viInfo, PositionInfo& info, bool isStatic) {
    ColorShape sha(info, viInfo, isStatic);
    std::shared_ptr<ColorNode> test = std::make_shared<ColorNode>(ColorNode(sha));
    batman.addShape(test);
    return test->getShape();
}
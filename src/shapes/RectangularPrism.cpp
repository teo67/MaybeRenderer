#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "ColorShape.h"
#include "RectangularPrism.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include "../BatchManager.h"


std::vector<glm::vec3> rectangularPrismVertices = {
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(-0.5f, 0.5f, -0.5f),
    glm::vec3(0.5f, 0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f, 0.5f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, 0.5f, 0.5f),
    glm::vec3(-0.5f, -0.5f, 0.5f)
};
std::vector<unsigned int> rectangularPrismIndices = {
    0, 1, 2, 
    0, 2, 3, // back

    0, 1, 6, 
    0, 6, 7, // left

    0, 3, 4, 
    0, 4, 7, // bottom 

    1, 2, 5, 
    1, 5, 6, // top

    2, 3, 4, 
    2, 4, 5, // right

    4, 5, 6, 
    4, 6, 7 // front
};
VertexIndexInfo vertexIndexInfo = VertexIndexInfo(rectangularPrismVertices, rectangularPrismIndices);
Vec2 defaultTextureInfo[8] = {
    Vec2(1.0f, 0.0f), Vec2(1.0f, 1.0f), Vec2(0.0f, 1.0f), Vec2(0.0f, 0.0f),
    Vec2(1.0f, 0.0f), Vec2(1.0f, 1.0f), Vec2(0.0f, 1.0f), Vec2(0.0f, 0.0f)
};
ColorShape& RectangularPrism::createColor(BatchManager& batman, PositionInfo& positionInfo, bool isStatic) {
    return RectangularPrism::create(batman, vertexIndexInfo, positionInfo, isStatic);
}
ColorShape& RectangularPrism::createColor(BatchManager& batman, bool isStatic) {
    PositionInfo pos;
    return RectangularPrism::createColor(batman, pos, isStatic);
}
ColorShape& RectangularPrism::createColor(BatchManager& batman) {
    return RectangularPrism::createColor(batman, false);
}
ColorShape& RectangularPrism::create(BatchManager& batman, VertexIndexInfo& viInfo, PositionInfo& posInfo, bool isStatic) {
    ColorShape sha(posInfo, viInfo, isStatic);
    std::shared_ptr<ColorNode> test = std::make_shared<ColorNode>(ColorNode(sha));
    batman.addShape(test);
    return test->getShape();
}
void RectangularPrism::test(std::shared_ptr<Node1>& in) {
    //std::cout << in->shape.getNumVertices() << std::endl;
}
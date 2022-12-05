#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "ColorShape.h"
#include "RectangularPrism.h"
#include <algorithm>
#include <iostream>
#include <vector>

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
ColorShape RectangularPrism::createColor(PositionInfo& positionInfo, bool isStatic) {
    return ColorShape(positionInfo, vertexIndexInfo, isStatic);
}
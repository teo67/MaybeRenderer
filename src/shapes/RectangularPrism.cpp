#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "RectangularPrism.h"
#include <algorithm>
#include <iostream>
unsigned int RectangularPrism::getNumVertices() {
    return 8;
}
unsigned int RectangularPrism::getNumIndices() {
    return 36;
}
void RectangularPrism::appendVertexData(float* vertexData, unsigned int index) {
    glm::mat4 matrix = Shape::getMatrix();
    fillMatrixData(vertexData, index, matrix, -0.5f, -0.5f, -0.5f);
    fillMatrixData(vertexData, index + 7, matrix, -0.5f, 0.5f, -0.5f);
    fillMatrixData(vertexData, index + 14, matrix, 0.5f, 0.5f, -0.5f);
    fillMatrixData(vertexData, index + 21, matrix, 0.5f, -0.5f, -0.5f);
    fillMatrixData(vertexData, index + 28, matrix, 0.5f, -0.5f, 0.5f);
    fillMatrixData(vertexData, index + 35, matrix, 0.5f, 0.5f, 0.5f);
    fillMatrixData(vertexData, index + 42, matrix, -0.5f, 0.5f, 0.5f);
    fillMatrixData(vertexData, index + 49, matrix, -0.5f, -0.5f, 0.5f);
    fillSameData(vertexData, index + 4, 7, 8, r);
    fillSameData(vertexData, index + 5, 7, 8, g);
    fillSameData(vertexData, index + 6, 7, 8, b);
}
void RectangularPrism::appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstPoint) {
    unsigned int newData[36] = {
        firstPoint, firstPoint + 1, firstPoint + 2, 
        firstPoint, firstPoint + 2, firstPoint + 3, // back

        firstPoint, firstPoint + 1, firstPoint + 6, 
        firstPoint, firstPoint + 6, firstPoint + 7, // left

        firstPoint, firstPoint + 3, firstPoint + 4, 
        firstPoint, firstPoint + 4, firstPoint + 7, // bottom 

        firstPoint + 1, firstPoint + 2, firstPoint + 5, 
        firstPoint + 1, firstPoint + 5, firstPoint + 6, // top

        firstPoint + 2, firstPoint + 3, firstPoint + 4, 
        firstPoint + 2, firstPoint + 4, firstPoint + 5, // right

        firstPoint + 4, firstPoint + 5, firstPoint + 6, 
        firstPoint + 4, firstPoint + 6, firstPoint + 7 // front
    };
    std::copy(&newData[0], &newData[36], &indexData[index]);
}
RectangularPrism::RectangularPrism(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale) {
    this->init(isStatic, x, y, z, pitch, yaw, roll, xScale, yScale, zScale);
}
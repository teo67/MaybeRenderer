#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../BatchInfo.h"
#include "Shape.h"
#include <iostream>

void Shape::init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale) {
    this->isStatic = isStatic;
    this->state = ShapeState::ENABLED;
    setPosition(x, y, z);
    setRotation(pitch, yaw, roll);
    setScale(xScale, yScale, zScale);
}
// void Shape::setStaticState(bool isStatic, Game game) {
//     if(this->isStatic != isStatic) {
//         this->isStatic = isStatic;
//         this->state = ShapeState::DISABLED_RESET; // act like disable perm, but switch back to enabled once done
//         this->deploy(game);
//     }
// }
const BatchInfo Shape::getFormat() {
    return isStatic ? STATIC_COLOR : DYNAMIC_COLOR;
}
bool Shape::reenable() {
    if(state == ShapeState::DISABLED_TEMPORARY) {
        state = ShapeState::ENABLED;
        return true;
    }
    return false;
}
void Shape::disable(bool permanent) {
    state = permanent ? ShapeState::DISABLED_PERMANENT : ShapeState::DISABLED_TEMPORARY;
}
ShapeState Shape::getState() {
    return state;
}
void Shape::appendVertexData(float* vertexData, unsigned int index) {
}
void Shape::appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstIndex) {
}
unsigned int Shape::getNumVertices() {
    return 0;
}
unsigned int Shape::getNumIndices() {
    return 0;
}
void Shape::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}
void Shape::translate(float dx, float dy, float dz) {
    position = glm::vec3(position.x + dx, position.y + dy, position.z + dz);
}
void Shape::setRotation(float pitch, float yaw, float roll) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
}
void Shape::setScale(float xScale, float yScale, float zScale) {
    scale = glm::vec3(xScale, yScale, zScale);
}
glm::mat4 Shape::getMatrix() {
    glm::mat4 matrix = glm::translate(glm::mat4(1.0), position);
    matrix = glm::scale(matrix, scale);
    matrix = glm::rotate(matrix, pitch, xv);
    matrix = glm::rotate(matrix, yaw, yv);
    matrix = glm::rotate(matrix, roll, zv);
    return matrix;
}
void Shape::fillMatrixData(float* data, unsigned int index, glm::mat4 matrix, float x, float y, float z) {
    glm::vec4 asVec = glm::vec4(x, y, z, 1.0);
    asVec = matrix * asVec;
    data[index] = asVec.x;
    data[index + 1] = asVec.y;
    data[index + 2] = asVec.z;
    data[index + 3] = asVec.w;
}
void Shape::fillSameData(float* data, unsigned int offset, unsigned int stride, unsigned int howMany, float value) {
    for(int i = 0; i < howMany; i++) {
        data[offset + i * stride] = value;
    }
}
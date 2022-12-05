#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
PositionInfo::PositionInfo(float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale) {
    position = glm::vec3(x, y, z);
    scale = glm::vec3(xScale, yScale, zScale);
    this->pitch = pitch;
    this->yaw = yaw;
    this->roll = roll;
}
PositionInfo::PositionInfo(float x, float y, float z) : PositionInfo(x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f) {}
PositionInfo::PositionInfo() : PositionInfo(0.0f, 0.0f, 0.0f) {}
VertexIndexInfo::VertexIndexInfo(std::vector<glm::vec3>& _vertices, std::vector<unsigned int>& _indices)
: vertices(_vertices), indices(_indices) {}
Vec2::Vec2(float x, float y) {
    this->x = x;
    this->y = y;
}
Shape::Shape(PositionInfo& _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
    positionInfo(_positionInfo),
    vertexIndexInfo(_vertexIndexInfo),
    isStatic(_isStatic),
    state(ShapeState::ENABLED) {}

glm::mat4 Shape::getMatrix() {
    glm::mat4 matrix = glm::translate(glm::mat4(1.0), positionInfo.position);
    matrix = glm::scale(matrix, positionInfo.scale);
    matrix = glm::rotate(matrix, positionInfo.pitch, xv);
    matrix = glm::rotate(matrix, positionInfo.yaw, yv);
    matrix = glm::rotate(matrix, positionInfo.roll, zv);
    return matrix;
}

void Shape::fillSameValue(std::vector<float>& data, unsigned int offset, unsigned int stride, float value) {
    for(int i = 0; i < vertexIndexInfo.vertices.size(); i++) {
        data[offset + i * stride] = value;
    }
    
}

void Shape::fillPositionInfo(std::vector<float>& data, unsigned int offset, unsigned int stride) {
    glm::mat4 matrix = getMatrix();
    for(int i = 0; i < vertexIndexInfo.vertices.size(); i++) {
        glm::vec4 result = matrix * glm::vec4(vertexIndexInfo.vertices[i], 1.0f);
        data[offset + i * stride] = result.x;
        data[offset + i * stride + 1] = result.y;
        data[offset + i * stride + 2] = result.z;
        data[offset + i * stride + 3] = result.w;
    }
}

void Shape::appendIndexData(unsigned int* data, unsigned int index, unsigned int firstIndex) {
    for(int i = 0; i < vertexIndexInfo.indices.size(); i++) {
        data[index + i] = vertexIndexInfo.indices[i] + firstIndex;
    }
}

unsigned int Shape::getNumVertices() {
    return vertexIndexInfo.vertices.size();
}
unsigned int Shape::getNumIndices() {
    return vertexIndexInfo.indices.size();
}

void Shape::appendVertexData(std::vector<float>& vertexData, unsigned int index) {
    std::cout << "APPENDING EMPTY VERTEX DATA!!" << std::endl;
}
ShapeState Shape::getState() {
    return state;
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

const BatchInfo& Shape::getFormat() {
    return isStatic ? STATIC_COLOR : DYNAMIC_COLOR;
}
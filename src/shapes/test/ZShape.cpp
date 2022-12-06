// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include "../../BatchInfo.h"
// #include "ZShape.h"
// #include <iostream>

// void ZShape::init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale) {
//     this->isStatic = isStatic;
//     this->state = ZShapeState::ENABLED;
//     setPosition(x, y, z);
//     setRotation(pitch, yaw, roll);
//     setScale(xScale, yScale, zScale);
// }
// ZShape::ZShape() {
//     init(true, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
// }
// // void ZShape::setStaticState(bool isStatic, Game game) {
// //     if(this->isStatic != isStatic) {
// //         this->isStatic = isStatic;
// //         this->state = ZShapeState::DISABLED_RESET; // act like disable perm, but switch back to enabled once done
// //         this->deploy(game);
// //     }
// // }
// const BatchInfo ZShape::getFormat() {
//     return isStatic ? STATIC_COLOR : DYNAMIC_COLOR;
// }
// bool ZShape::reenable() {
//     if(state == ZShapeState::DISABLED_TEMPORARY) {
//         state = ZShapeState::ENABLED;
//         return true;
//     }
//     return false;
// }
// void ZShape::disable(bool permanent) {
//     state = permanent ? ZShapeState::DISABLED_PERMANENT : ZShapeState::DISABLED_TEMPORARY;
// }
// ZShapeState ZShape::getState() {
//     return state;
// }
// void ZShape::appendVertexData(float* vertexData, unsigned int index) {
// }
// void ZShape::appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstIndex) {
// }
// unsigned int ZShape::getNumVertices() {
//     return 0;
// }
// unsigned int ZShape::getNumIndices() {
//     return 0;
// }
// void ZShape::setPosition(float x, float y, float z) {
//     position = glm::vec3(x, y, z);
// }
// void ZShape::translate(float dx, float dy, float dz) {
//     position = glm::vec3(position.x + dx, position.y + dy, position.z + dz);
// }
// void ZShape::setRotation(float pitch, float yaw, float roll) {
//     this->pitch = pitch;
//     this->yaw = yaw;
//     this->roll = roll;
// }
// void ZShape::setScale(float xScale, float yScale, float zScale) {
//     scale = glm::vec3(xScale, yScale, zScale);
// }
// glm::mat4 ZShape::getMatrix() {
//     glm::mat4 matrix = glm::translate(glm::mat4(1.0), position);
//     matrix = glm::scale(matrix, scale);
//     matrix = glm::rotate(matrix, pitch, zxv);
//     matrix = glm::rotate(matrix, yaw, zyv);
//     matrix = glm::rotate(matrix, roll, zzv);
//     return matrix;
// }
// void ZShape::fillMatrixData(float* data, unsigned int index, glm::mat4 matrix, float x, float y, float z) {
//     glm::vec4 asVec = glm::vec4(x, y, z, 1.0);
//     asVec = matrix * asVec;
//     data[index] = asVec.x;
//     data[index + 1] = asVec.y;
//     data[index + 2] = asVec.z;
//     data[index + 3] = asVec.w;
// }
// void ZShape::fillSameData(float* data, unsigned int offset, unsigned int stride, unsigned int howMany, float value) {
//     for(int i = 0; i < howMany; i++) {
//         data[offset + i * stride] = value;
//     }
// }
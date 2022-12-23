#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mouse.h"
#include "Camera.h"
void Camera::initDefault() {
    this->yaw = defaultYaw;
    this->pitch = defaultPitch;
    this->sensitivity = defaultSensitivity;
    this->position = defaultPosition;
    this->updateVectors();
}
void Camera::updateVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->forwardVector = glm::normalize(direction);
    this->rightVector = glm::normalize(glm::cross(this->forwardVector, UP));
}
Camera::Camera() {
    this->initDefault();
}

void Camera::forward(float dt) {
    position += forwardVector * dt;
}
void Camera::backward(float dt) {
    position -= forwardVector * dt;
}
void Camera::right(float dt) {
    position += rightVector * dt;
}
void Camera::left(float dt) {
    position -= rightVector * dt;
}
void Camera::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}
void Camera::setRotation(float _yaw, float _pitch) {
    yaw = _yaw;
    pitch = _pitch;
    updateVectors();
}
void Camera::setYaw(float _yaw) {
    yaw = _yaw;
    updateVectors();
}
void Camera::setPitch(float _pitch) {
    pitch = _pitch;
    updateVectors();
}
void Camera::handleMouse(Mouse mouse) {
    float xoffset = mouse.getDx() * sensitivity;
    float yoffset = mouse.getDy() * sensitivity;
    yaw += xoffset;
    pitch += yoffset;  
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    this->updateVectors();
}
glm::mat4 Camera::getView() {
    // std::cout << forwardVector.x << " " << forwardVector.y << " " << forwardVector.z << std::endl;
    return glm::lookAt(position, position + forwardVector, UP);
}
void Camera::setSensitivity(float sens) {
    sensitivity = sens;
}
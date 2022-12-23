#include <iostream>
#include "Mouse.h"
Mouse::Mouse() {
    this->x = 0;
    this->y = 0;
    this->dx = 0;
    this->dy = 0;
    this->firstMouse = true;
}
double Mouse::getX() {
    return x;
}
double Mouse::getY() {
    return y;
}
double Mouse::getDx() {
    return dx;
}
double Mouse::getDy() {
    return dy;
}
void Mouse::handleInput(double xpos, double ypos) {
    if (firstMouse) {
        x = xpos;
        y = ypos;
        firstMouse = false;
    }
    dx = xpos - x;
    dy = y - ypos;
    x = xpos;
    y = ypos;
}
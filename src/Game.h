#ifndef GAME_H
#define GAME_H
#include "BatchManager.h"
#include "ShapeManager.h"
#include "TextureManager.h"
#include "shapes/ColorShape.h"
#include "shapes/MulticolorShape.h"
#include "shapes/TexturedShape.h"
#include "shapes/MultitexturedShape.h"
#include "Mouse.h"
#include "Camera.h"
#include <map>
#include <string>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
class Game {
    private:
        BatchManager batman;
        glm::mat4 projectionMatrix;
        float _dt;
        float previousTime;
        GLFWwindow* window;
        unsigned int width;
        unsigned int height;
        float fov;
        void updateProjectionMatrix();
    public:
        Game(unsigned int width, unsigned int height);
        ~Game();
        ShapeManager shaman;
        TextureManager texman; // i couldn't think of a good name :(((
        Mouse mouse;
        Camera camera;
        Color backgroundColor;
        void render();
        ColorShape& generateColorShape(const VertexIndexInfo& viInfo, bool isStatic);
        MulticolorShape& generateMulticolorShape(const VertexIndexInfo& viInfo, bool isStatic);
        TexturedShape& generateTexturedShape(const VertexIndexInfo& viInfo, bool isStatic);
        MultitexturedShape& generateMultitexturedShape(const VertexIndexInfo& viInfo, bool isStatic);
        float dt();
        float time();
        bool isKeyPressed(int key);
        void close();
        bool closed();
        void resizeWindow(unsigned int width, unsigned int height);
        void respondToMouse();
        void respondToScroll(double x, double y);
        void setFOV(float fov);
};
#endif
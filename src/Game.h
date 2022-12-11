#ifndef GAME_H
#define GAME_H
#include "BatchManager.h"
#include "ShapeManager.h"
#include "shapes/ColorShape.h"
#include "shapes/MulticolorShape.h"
#include "shapes/TexturedShape.h"
#include "Mouse.h"
#include "Camera.h"
#include <map>
#include <string>
#include <GLFW/glfw3.h>
class Game {
    private:
        BatchManager batman;
        std::map<std::string, unsigned int> textureMap;
        void setTextureToImage(std::string path);
        glm::mat4 projectionMatrix;
        float _dt;
        float previousTime;
        GLFWwindow* window;
    public:
        Game(unsigned int width, unsigned int height);
        ~Game();
        ShapeManager shaman;
        Mouse mouse;
        Camera camera;
        Color backgroundColor;
        void render();
        ColorShape& generateColorShape(const VertexIndexInfo& viInfo, bool isStatic);
        MulticolorShape& generateMulticolorShape(const VertexIndexInfo& viInfo, bool isStatic);
        TexturedShape& generateTexturedShape(const VertexIndexInfo& viInfo, bool isStatic, unsigned int tex);
        TexturedShape& generateTexturedShape(const VertexIndexInfo& viInfo, bool isStatic, std::string texName);
        void registerTexture(std::string name, std::string path);
        void changeTextureImage(std::string name, std::string path);
        void removeTexture(std::string name);
        unsigned int getTexture(std::string name);
        float dt();
        float time();
        void updateProjectionMatrix(unsigned int width, unsigned int height);
        bool isKeyPressed(int key);
        void close();
        bool closed();

        void respondToMouse();
        void respondToScroll(double x, double y);
};
#endif
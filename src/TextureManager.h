#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <map>
#include <string>
#include <glad/glad.h>
class TextureManager {
    private: 
        std::map<std::string, GLuint> textureMap;
        void setTextureToImage(std::string path, bool a);
    public:
        TextureManager();
        void registerTexture(std::string name, std::string path, bool a);
        void changeTextureImage(std::string name, std::string path, bool a);
        void removeTexture(std::string name);
        GLuint getTexture(std::string name);
        void cleanup();
};
#endif
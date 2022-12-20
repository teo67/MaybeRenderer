#include "TextureManager.h"
#include <glad/glad.h>
#include <string>
#include <map>
#include <stb/stb_image.h>
#include <iostream>

GLuint TextureManager::getTexture(std::string name) {
    //std::cout << "tex: " << textureMap[name] << std::endl;
    return textureMap[name];
}

void TextureManager::setTextureToImage(std::string path, bool a) {
    int _width, _height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, a ? GL_RGBA : GL_RGB, _width, _height, 0, a ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void TextureManager::registerTexture(std::string name, std::string path, bool a) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    setTextureToImage(path, a);
    
    textureMap[name] = texture;
    
}

void TextureManager::changeTextureImage(std::string name, std::string path, bool a) {
    GLuint texture = getTexture(name);
    glBindTexture(GL_TEXTURE_2D, texture);
    setTextureToImage(path, a);
}

void TextureManager::removeTexture(std::string name) {
    glDeleteTextures(1, &textureMap[name]);
    textureMap.erase(name);
}

TextureManager::TextureManager() {
    stbi_set_flip_vertically_on_load(true); // the right way LMAO
    textureMap = {};
}

void TextureManager::cleanup() {
    textureMap = {};
}
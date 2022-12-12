#ifndef TEXTURESET_H
#define TEXTURESET_H
#include <map>
#include <vector>
#include <glad/glad.h>
struct TextureSet {
    static unsigned int maxTextureUnits; // !!!


    std::vector<std::map<GLuint, GLuint>> maps;
    unsigned int nextInt;
    std::vector<unsigned int> lastIndices;
    TextureSet();
    GLuint getTexture(GLuint key, unsigned int possibleNewLastIndex);
    void reset();
};
#endif
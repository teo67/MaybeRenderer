#ifndef TEXTURESET_H
#define TEXTURESET_H
#include <map>
#include <vector>
#include <glad/glad.h>
struct TextureSet {
    unsigned int maxTextureUnits;
    std::vector<std::map<unsigned int, unsigned int>> maps;
    unsigned int nextInt;
    std::vector<unsigned int> lastIndices;
    TextureSet(unsigned int maxTexturesPerSet);
    unsigned int getTexture(unsigned int key, unsigned int possibleNewLastIndex);
    void reset();
};
#endif
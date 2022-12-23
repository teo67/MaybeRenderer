#include "TextureSet.h"
#include "map"
#include <glad/glad.h>

unsigned int TextureSet::maxTextureUnits = 0;

TextureSet::TextureSet() {
    reset();
}

GLuint TextureSet::getTexture(GLuint texture, unsigned int possibleNewLastIndex) {
    if(maps.size() > 0) {
        std::map<GLuint, GLuint>& lastMap = maps.back();
        std::map<GLuint, GLuint>::const_iterator found = lastMap.find(texture);
        if(found != lastMap.end()) {
            return found->second;
        }
        if(nextInt < maxTextureUnits) {
            lastMap[texture] = nextInt;
            return nextInt++;
        }
        lastIndices.push_back(possibleNewLastIndex);
    }
    nextInt = 1;
    maps.push_back({{texture, 0}});
    return 0;
}

void TextureSet::reset() {
    maps = {};
    nextInt = 0;
    lastIndices = {};
}
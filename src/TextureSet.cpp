#include "TextureSet.h"
#include "map"
TextureSet::TextureSet(unsigned int maxTexturesPerSet) {
    maxTextureUnits = maxTexturesPerSet;
    reset();
}

unsigned int TextureSet::getTexture(unsigned int texture, unsigned int possibleNewLastIndex) {
    if(maps.size() > 0) {
        std::map<unsigned int, unsigned int>& lastMap = maps.back();
        std::map<unsigned int, unsigned int>::const_iterator found = lastMap.find(texture);
        if(found != lastMap.end()) {
            return found->second;
        }
        if(nextInt < maxTextureUnits) {
            lastMap[texture] = nextInt;
            return nextInt++;
        }
        lastIndices.push_back(possibleNewLastIndex);
    }
    nextInt = 0;
    maps.push_back({});
    return 0;
}

void TextureSet::reset() {
    maps = {};
    nextInt = 0;
    lastIndices = {};
}
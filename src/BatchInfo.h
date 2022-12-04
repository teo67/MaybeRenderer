#ifndef BATCHINFO_H
#define BATCHINFO_H
#include <vector>
struct BatchInfo {
    unsigned int index;
    std::vector<unsigned int> sizes;
    bool isStatic;
    BatchInfo(unsigned int index, std::vector<unsigned int> sizes, bool isStatic) {
        this->index = index;
        this->sizes = sizes;
        this->isStatic = isStatic;
    }
};
inline const BatchInfo STATIC_COLOR(0, std::vector<unsigned int>{ 4, 3 }, true);
inline const BatchInfo DYNAMIC_COLOR(1, std::vector<unsigned int>{ 4, 3 }, false);
inline const BatchInfo STATIC_TEXTURE(2, std::vector<unsigned int>{ 4, 2, 1 }, true);
inline const BatchInfo DYNAMIC_TEXTURE(3, std::vector<unsigned int>{ 4, 2, 1 }, false);
inline const BatchInfo STATIC_COLORTEXTURE(4, std::vector<unsigned int>{ 4, 3, 2, 1 }, true);
inline const BatchInfo DYNAMIC_COLORTEXTURE(5, std::vector<unsigned int>{ 4, 3, 2, 1 }, false);
inline const unsigned int NUM_BATCHES = 6;
#endif
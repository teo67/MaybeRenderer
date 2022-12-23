#include "ShapeManager.h"
#include "BatchManager.h"
#include "../shapes/Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include <iostream>

ShapeManager::ShapeManager(float radius) {
    this->radius = radius;
    polygons = {};
    prisms = {};
    pyramids = {};
}

ShapeManager::ShapeManager() : ShapeManager(defaultRadius) {}

void ShapeManager::generateRegularPolygon(unsigned int numSides, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& faceNumbers, std::vector<unsigned int>& faceIndexOffsets) {
    for(unsigned int i = 0; i < numSides; i++) {
        float theta = 360 * i / numSides + 45; // add 45 to make squares nicer
        float xcomp = sinf(glm::radians(theta));
        float zcomp = cosf(glm::radians(theta));
        vertices[i] = glm::vec3(radius * xcomp, 0.0f, radius * zcomp);
        if(numSides == 4) {
            xcomp *= sqrt(2.0f);
            zcomp *= sqrt(2.0f); // special case for squares to fit textures nicely
        }
        texCoords[i] = glm::vec2((xcomp + 1) / 2, (zcomp + 1) / 2);
        faceNumbers[i] = 0;
    }
    unsigned int index = 0;
    for(unsigned int multiplier = 1; multiplier * 2 < numSides; multiplier *= 2) { // add all base plate triangles
        for(unsigned int i = 0; i + multiplier < numSides; i += multiplier * 2) {
            unsigned int lastIndex = i + 2 * multiplier;
            if(lastIndex >= numSides) {
                lastIndex = 0;
            }
            indices[index] = i;
            indices[index + 1] = i + multiplier;
            indices[index + 2] = lastIndex;
            index += 3;
        }
    }
    faceIndexOffsets[0] = 0;
}

const VertexIndexInfo& ShapeManager::createRegularPolygon(unsigned int numSides) {
    std::vector<glm::vec3> vertices(numSides);
    std::vector<unsigned int> indices(3 * numSides - 6);
    std::vector<glm::vec2> coords(numSides);
    std::vector<unsigned int> faceNumbers(numSides);
    std::vector<unsigned int> faceIndexOffsets(1);
    generateRegularPolygon(numSides, vertices, indices, coords, faceNumbers, faceIndexOffsets);
    return polygons.insert({ numSides, VertexIndexInfo(vertices, indices, coords, faceNumbers, faceIndexOffsets )}).first->second;
}

const VertexIndexInfo& ShapeManager::createPrism(unsigned int numSides, bool split) {
    std::vector<glm::vec3> vertices(split ? numSides * 6 : numSides * 2);
    std::vector<glm::vec2> coords(split ? numSides * 6 : numSides * 2);
    std::vector<unsigned int> indices(12 * numSides - 12);
    std::vector<unsigned int> faceNumbers(split ? numSides * 6 : numSides * 2);
    std::vector<unsigned int> faceIndexOffsets(split ? 2 + numSides : 1);
    generateRegularPolygon(numSides, vertices, indices, coords, faceNumbers, faceIndexOffsets);
    float height = sqrt(2.0f) * radius; // so that rectangular prisms are cubes by default
    unsigned int index = 3 * numSides - 6;
    for(int i = 0; i < 3 * numSides - 6; i++) {
        indices[index] = indices[i] + numSides;
        index++;
    }
    for(int i = 0; i < numSides; i++) {
        vertices[numSides + i] = vertices[i];
        vertices[numSides + i].y = height;
        coords[numSides + i] = coords[i];
        faceNumbers[numSides + i] = split ? 1 : 0;
        if(split) {
            vertices[2 * numSides + i] = vertices[i];
            coords[2 * numSides + i] = glm::vec2(0.0f, 0.0f);
            faceNumbers[2 * numSides + i] = 2 + i;
            vertices[3 * numSides + i] = vertices[numSides + i];
            coords[3 * numSides + i] = glm::vec2(0.0f, 1.0f);
            faceNumbers[3 * numSides + i] = 2 + i;
            vertices[4 * numSides + i] = vertices[i];
            coords[4 * numSides + i] = glm::vec2(1.0f, 0.0f);
            faceNumbers[4 * numSides + i] = (i == 0) ? (numSides + 1) : (1 + i);
            vertices[5 * numSides + i] = vertices[numSides + i];
            coords[5 * numSides + i] = glm::vec2(1.0f, 1.0f);
            faceNumbers[5 * numSides + i] = (i == 0) ? (numSides + 1) : (1 + i);

            indices[index] = 2 * numSides + i; // add side plates, two triangles at a time
            indices[index + 1] = 4 * numSides + ((i == numSides - 1) ? 0 : (i + 1));
            indices[index + 2] = i + 3 * numSides;
            indices[index + 3] = i + 3 * numSides;
            indices[index + 4] = 5 * numSides + ((i == numSides - 1) ? 0 : (i + 1));
            indices[index + 5] = 4 * numSides + ((i == numSides - 1) ? 0 : (i + 1));
        } else {
            indices[index] = i; // add side plates, two triangles at a time
            indices[index + 1] = (i == numSides - 1) ? 0 : (i + 1);
            indices[index + 2] = i + numSides;
            indices[index + 3] = i + numSides;
            indices[index + 4] = (i == numSides - 1) ? numSides : (i + numSides + 1);
            indices[index + 5] = (i == numSides - 1) ? 0 : (i + 1);
        }
        index += 6;
    }
    if(split) {
        faceIndexOffsets[1] = 3 * numSides - 6;
        for(int i = 0; i < numSides; i++) {
            faceIndexOffsets[i + 2] = 6 * numSides - 12 + 6 * i;
        }
    }
    return (split ? splitprisms : prisms).insert({ numSides, VertexIndexInfo(vertices, indices, coords, faceNumbers, faceIndexOffsets )}).first->second;
}

const VertexIndexInfo& ShapeManager::createPyramid(unsigned int numSides, bool split) {
    std::vector<glm::vec3> vertices(split ? 4 * numSides : numSides + 1);
    std::vector<glm::vec2> coords(split ? 4 * numSides : numSides + 1);
    std::vector<unsigned int> indices(6 * numSides - 6);
    std::vector<unsigned int> faceNumbers(split ? 4 * numSides : numSides + 1);
    std::vector<unsigned int> faceIndexOffsets(split ? numSides + 1 : 1);
    generateRegularPolygon(numSides, vertices, indices, coords, faceNumbers, faceIndexOffsets);
    float height = sqrt(2.0f) * radius; // so that triangular pyramids are regular by default
    glm::vec3 top = glm::vec3(0.0f, height, 0.0f);
    unsigned int index = 3 * numSides - 6;
    if(!split) {
        vertices[numSides] = top;
        coords[numSides] = glm::vec2(0.0f, 0.0f);
        faceNumbers[numSides] = 0;
    }
    for(int i = 0; i < numSides; i++) {
        if(split) {
            vertices[numSides + i] = vertices[i];
            faceNumbers[numSides + i] = i + 1;
            vertices[2 * numSides + i] = vertices[i];
            faceNumbers[2 * numSides + i] = (i == 0) ? numSides : i;
            vertices[3 * numSides + i] = top;
            faceNumbers[3 * numSides + i] = i + 1;
            coords[numSides + i] = glm::vec2(0.0f, 0.0f);
            coords[2 * numSides + i] = glm::vec2(1.0f, 0.0f);
            coords[3 * numSides + i] = glm::vec2(0.5f, 1.0f);
            indices[index] = numSides + i;
            indices[index + 1] = 2 * numSides + ((i == numSides - 1) ? 0 : (i + 1));
            indices[index + 2] = 3 * numSides + i;

            faceIndexOffsets[i + 1] = 3 * numSides - 6 + 3 * i;
        } else {
            indices[index] = i;
            indices[index + 1] = (i == numSides - 1) ? 0 : (i + 1);
            indices[index + 2] = numSides;
        }
        index += 3;
    }
    return (split ? splitpyramids : pyramids).insert({ numSides, VertexIndexInfo(vertices, indices, coords, faceNumbers, faceIndexOffsets )}).first->second;
}

std::map<unsigned int, VertexIndexInfo>::const_iterator ShapeManager::findItemInMap(std::map<unsigned int, VertexIndexInfo>& map, unsigned int index, bool& worked) {
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = map.find(index);
    worked = found != map.end();
    return found;
}

const VertexIndexInfo& ShapeManager::prism(unsigned int numSides, bool split) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(split ? splitprisms : prisms, numSides, worked);
    if(!worked) {
        return createPrism(numSides, split);
    }
    return found->second;
}

const VertexIndexInfo& ShapeManager::polygon(unsigned int numSides) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(polygons, numSides, worked);
    if(!worked) {
        return createRegularPolygon(numSides);
    }
    return found->second;
}

const VertexIndexInfo& ShapeManager::pyramid(unsigned int numSides, bool split) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(split ? splitpyramids : pyramids, numSides, worked);
    if(!worked) {
        return createPyramid(numSides, split);
    }
    return found->second;
}

void ShapeManager::cleanup() {
    polygons = {};
    prisms = {};
    pyramids = {};
}
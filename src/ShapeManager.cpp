#include "ShapeManager.h"
#include "BatchManager.h"
#include "shapes/Shape.h"
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

void ShapeManager::generateRegularPolygon(unsigned int numSides, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) {
    for(unsigned int i = 0; i < numSides; i++) {
        float theta = 360 * i / numSides;
        float xcomp = radius * sinf(glm::radians(theta));
        float zcomp = radius * cosf(glm::radians(theta));
        vertices[i] = glm::vec3(xcomp, 0.0f, zcomp);
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
}

const VertexIndexInfo& ShapeManager::createRegularPolygon(unsigned int numSides) {
    std::vector<glm::vec3> vertices(numSides);
    std::vector<unsigned int> indices(3 * numSides - 6);
    generateRegularPolygon(numSides, vertices, indices);
    return polygons.insert({ numSides, VertexIndexInfo(vertices, indices )}).first->second;
}

const VertexIndexInfo& ShapeManager::createPrism(unsigned int numSides) {
    std::vector<glm::vec3> vertices(numSides * 2);
    std::vector<unsigned int> indices(12 * numSides - 12);
    generateRegularPolygon(numSides, vertices, indices);
    float height = sqrt(2.0f) * radius; // so that rectangular prisms are cubes by default
    unsigned int index = 3 * numSides - 6;
    for(int i = 0; i < numSides; i++) {
        vertices[numSides + i] = vertices[i];
        vertices[numSides + i].y = height;
        indices[index] = i; // add side plates, two triangles at a time
        indices[index + 1] = (i == numSides - 1) ? 0 : (i + 1);
        indices[index + 2] = i + numSides;
        indices[index + 3] = i + numSides;
        indices[index + 4] = (i == numSides - 1) ? numSides : (i + numSides + 1);
        indices[index + 5] = (i == numSides - 1) ? 0 : (i + 1);
        index += 6;
    }
    for(int i = 0; i < 3 * numSides - 6; i++) {
        indices[index] = indices[i] + numSides;
        index++;
    }
    return prisms.insert({ numSides, VertexIndexInfo(vertices, indices )}).first->second;
}

const VertexIndexInfo& ShapeManager::createPyramid(unsigned int numSides) {
    std::vector<glm::vec3> vertices(numSides + 1);
    std::vector<unsigned int> indices(6 * numSides - 6);
    generateRegularPolygon(numSides, vertices, indices);
    float height = sqrt(2.0f) * radius; // so that triangular pyramids are regular by default
    vertices[numSides] = glm::vec3(0.0f, height, 0.0f);
    unsigned int index = 3 * numSides - 6;
    for(int i = 0; i < numSides; i++) {
        indices[index] = i;
        indices[index + 1] = (i == numSides - 1) ? 0 : (i + 1);
        indices[index + 2] = numSides;
        index += 3;
    }
    const VertexIndexInfo& returning = pyramids.insert({ numSides, VertexIndexInfo(vertices, indices )}).first->second;
    return returning;
}

std::map<unsigned int, VertexIndexInfo>::const_iterator ShapeManager::findItemInMap(std::map<unsigned int, VertexIndexInfo>& map, unsigned int index, bool& worked) {
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = map.find(index);
    worked = found != map.end();
    return found;
}

const VertexIndexInfo& ShapeManager::prism(unsigned int numSides) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(prisms, numSides, worked);
    if(!worked) {
        return createPrism(numSides);
    }
    const VertexIndexInfo& res = found->second;
    printVI(res);
    return res;
}

const VertexIndexInfo& ShapeManager::polygon(unsigned int numSides) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(polygons, numSides, worked);
    if(!worked) {
        return createRegularPolygon(numSides);
    }
    const VertexIndexInfo& res = found->second;
    printVI(res);
    return res;
}

const VertexIndexInfo& ShapeManager::pyramid(unsigned int numSides) {
    bool worked = false;
    std::map<unsigned int, VertexIndexInfo>::const_iterator found = findItemInMap(pyramids, numSides, worked);
    if(!worked) {
        return createPyramid(numSides);
    }
    const VertexIndexInfo& res = found->second;
    printVI(res);
    return res;
}

void ShapeManager::printVI(const VertexIndexInfo& info) {
    std::cout << "Vertices: " << std::endl;
    for(int i = 0; i < info.vertices.size(); i++) {
        std::cout << i << ": " << info.vertices[i].x << ", " << info.vertices[i].y << ", " << info.vertices[i].z << std::endl;
    }
    std::cout << "Indices: " << std::endl;
    for(int i = 0; i < info.indices.size(); i += 3) {
        std::cout << info.indices[i] << ", " << info.indices[i + 1] << ", " << info.indices[i + 2] << std::endl;
    }
}

void ShapeManager::cleanup() {
    polygons = {};
    prisms = {};
    pyramids = {};
}
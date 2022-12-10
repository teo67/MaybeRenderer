#ifndef SHAPEMANAGER_H
#define SHAPEMANAGER_H
#include "shapes/Shape.h"
#include "shapes/ColorShape.h"
#include "BatchManager.h"
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
const inline float defaultRadius = 0.5f;
class ShapeManager {
    private:
        float radius;
        float height;
        std::map<unsigned int, VertexIndexInfo> polygons;
        std::map<unsigned int, VertexIndexInfo> prisms;
        std::map<unsigned int, VertexIndexInfo> pyramids;
        const VertexIndexInfo& createRegularPolygon(unsigned int numSides);
        const VertexIndexInfo& createPrism(unsigned int numSides);
        const VertexIndexInfo& createPyramid(unsigned int numSides);
        std::map<unsigned int, VertexIndexInfo>::const_iterator findItemInMap(std::map<unsigned int, VertexIndexInfo>& map, unsigned int item, bool& worked);
        void generateRegularPolygon(unsigned int numSides, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);
        void printVI(const VertexIndexInfo& info);
    public:
        const VertexIndexInfo& polygon(unsigned int numSides);
        const VertexIndexInfo& prism(unsigned int numSides);
        const VertexIndexInfo& pyramid(unsigned int numSides);
        ShapeManager(float radius);
        ShapeManager();
        void cleanup();
};
#endif
#ifndef SHAPE_H
#define SHAPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../BatchInfo.h"
enum class ShapeState : unsigned char {
    ENABLED, DISABLED_TEMPORARY, DISABLED_PERMANENT, DISABLED_RESET
};
inline const glm::vec3 xv = glm::vec3(1.0f, 0.0f, 0.0f);
inline const glm::vec3 yv = glm::vec3(0.0f, 1.0f, 0.0f);
inline const glm::vec3 zv = glm::vec3(0.0f, 0.0f, 1.0f);
struct PositionInfo {
    glm::vec3 position;
    glm::vec3 scale;
    float pitch;
    float yaw;
    float roll;
    PositionInfo(float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
    PositionInfo(float x, float y, float z);
    PositionInfo();
};
struct Vec2 {
    float x;
    float y;
    Vec2(float x, float y);
};
struct VertexIndexInfo {
    std::vector<glm::vec3>& vertices;
    std::vector<unsigned int>& indices;
    VertexIndexInfo(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices);
};
class Shape {
    private:
        PositionInfo& positionInfo;
        //Vec2* textureCoordinateInfo;
        VertexIndexInfo& vertexIndexInfo;
        ShapeState state;
        bool isStatic;
        glm::mat4 getMatrix();
    protected:
        void fillPositionInfo(std::vector<float>& data, unsigned int offset, unsigned int stride);
        void fillSameValue(std::vector<float>& data, unsigned int offset, unsigned int stride, float value);
    public:
        Shape(PositionInfo& _positionInfo, VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        virtual void appendVertexData(std::vector<float>& vertexData, unsigned int index);
        void appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstIndex);
        unsigned int getNumVertices();
        unsigned int getNumIndices();
        ShapeState getState();
        void disable(bool permanent);
        bool reenable();
        virtual const BatchInfo& getFormat();
};
#endif
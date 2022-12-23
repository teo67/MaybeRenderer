#ifndef SHAPE_H
#define SHAPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../stuff/BatchInfo.h"
#include <memory>
#include "../stuff/TextureSet.h"
enum class ShapeState : unsigned char {
    ENABLED, DISABLED_TEMPORARY, DISABLED_PERMANENT, DISABLED_RESET
};
inline const glm::vec3 xv = glm::vec3(1.0f, 0.0f, 0.0f);
inline const glm::vec3 yv = glm::vec3(0.0f, 1.0f, 0.0f);
inline const glm::vec3 zv = glm::vec3(0.0f, 0.0f, 1.0f);
struct PositionInfo {
    float x;
    float y;
    float z;
    float scaleX;
    float scaleY;
    float scaleZ;
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
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoordinates;
    std::vector<unsigned int> faceNumbers;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> faceIndexOffsets;
    void print() const;
    VertexIndexInfo(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec2>& texCoordinates, std::vector<unsigned int>& faceNumbers, std::vector<unsigned int>& faceIndexOffsets);
};

class Shape {
    private:
        ShapeState state;
    protected:
        void fillPositionInfo(std::vector<float>& data, unsigned int offset, unsigned int stride);
        void fillSameValue(std::vector<float>& data, unsigned int offset, unsigned int stride, float value);
        glm::mat4 getMatrix();
        const VertexIndexInfo& vertexIndexInfo;
        bool isStatic;
    public:
        PositionInfo transform;
        void setPosition(float x, float y, float z);
        void setRotation(float pitch, float yaw, float roll);
        void setScale(float x, float y, float z);
        Shape(PositionInfo _positionInfo, const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        virtual void appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex);
        void appendIndexData(std::vector<unsigned int>& indexData, unsigned int index, unsigned int firstIndex);
        unsigned int getNumVertices();
        unsigned int getNumIndices();
        ShapeState getState();
        void disable(bool permanent);
        bool reenable();
        virtual const BatchInfo& getFormat();
};
class Node1 {
    public:
        std::shared_ptr<Node1> next;
        Node1();
        virtual Shape& getShape();
};

#endif
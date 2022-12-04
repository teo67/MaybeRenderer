#ifndef SHAPE_H
#define SHAPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../BatchInfo.h"
enum class ShapeState {
    ENABLED, DISABLED_TEMPORARY, DISABLED_PERMANENT, DISABLED_RESET
};
inline const glm::vec3 xv = glm::vec3(1.0f, 0.0f, 0.0f);
inline const glm::vec3 yv = glm::vec3(0.0f, 1.0f, 0.0f);
inline const glm::vec3 zv = glm::vec3(0.0f, 0.0f, 1.0f);
class Shape {
    private:
        ShapeState state;
        glm::vec3 position;
        glm::vec3 scale;
        bool isStatic;
        float pitch;
        float yaw;
        float roll;
    protected:
        virtual void init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
        glm::mat4 getMatrix();
        void fillMatrixData(float* data, unsigned int index, glm::mat4 matrix, float x, float y, float z);
        void fillSameData(float* data, unsigned int offset, unsigned int stride, unsigned int howMany, float value);
    public:
        void setPosition(float x, float y, float z);
        void translate(float dx, float dy, float dz);
        void setRotation(float pitch, float yaw, float roll);
        void setScale(float xScale, float yScale, float zScale);
        virtual void appendVertexData(float* vertexData, unsigned int index);
        virtual void appendIndexData(unsigned int* indexData, unsigned int index, unsigned int firstIndex);
        virtual unsigned int getNumVertices();
        virtual unsigned int getNumIndices();
        ShapeState getState();
        void disable(bool permanent);
        bool reenable();
        const BatchInfo getFormat();
        //void setStaticState(bool isStatic, Game game);
};
#endif
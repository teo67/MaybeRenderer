#ifndef BATCH_H
#define BATCH_H
#include "shapes/Shape.h"
#include <glad/glad.h>
#include <memory>
inline const unsigned int defaultVertexSize = 1000;
inline const unsigned int defaultIndexSize = 1000;
inline const unsigned int defaultMaxNewShapesPerFrame = 0; // 0 = no max

class Batch {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        std::shared_ptr<Node1> firstShape;
        std::shared_ptr<Node1> lastShape;
        std::shared_ptr<Node1> firstShapeTBA;
        std::shared_ptr<Node1> lastShapeTBA;
        bool isStatic;
        unsigned int numIndices;
        unsigned int numVertices;
        unsigned int singleSize;
        void editVertexBuffer(unsigned int offset, std::vector<float>& vertices, unsigned int sizeo);
        void editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo);
        std::shared_ptr<Node1> popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices);
        unsigned int maxNewShapesPerFrame;
        void printVector(std::vector<float>& input);
    public:
        void init(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame);
        void init(std::vector<unsigned int> sizes, bool isStatic);
        Batch(); //creates batch w/o initializing
        void draw();
        void update();
        void addShapeToQueue(const std::shared_ptr<Node1>& shape);
        void end();
        bool marked;
        bool initialized;
};
#endif
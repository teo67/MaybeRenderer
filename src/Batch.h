#ifndef BATCH_H
#define BATCH_H
#include "shapes/Shape.h"
#include <glad/glad.h>
#include <memory>
struct Node {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Node> next;
    Node(std::shared_ptr<Shape>& _shape);
    Node();
};
inline const unsigned int defaultVertexSize = 1000;
inline const unsigned int defaultIndexSize = 1000;
inline const unsigned int defaultMaxNewShapesPerFrame = 0; // 0 = no max

class Batch {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        std::shared_ptr<Node> firstShape;
        std::shared_ptr<Node> lastShape;
        std::shared_ptr<Node> firstShapeTBA;
        std::shared_ptr<Node> lastShapeTBA;
        bool isStatic;
        unsigned int numIndices;
        unsigned int numVertices;
        unsigned int singleSize;
        void editVertexBuffer(unsigned int offset, std::vector<float>& vertices, unsigned int sizeo);
        void editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo);
        std::shared_ptr<Node> popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices);
        unsigned int maxNewShapesPerFrame;
        void init(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame);
        void printVector(std::vector<float>& input);
    public:
        Batch(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame);
        Batch(std::vector<unsigned int>, bool isStatic);
        void draw();
        void update();
        void addShapeToQueue(std::shared_ptr<Shape>& shape);
        void end();
        bool marked;
};
#endif
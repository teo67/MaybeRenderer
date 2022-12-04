#ifndef BATCH_H
#define BATCH_H
#include "shapes/Shape.h"
#include <glad/glad.h>
#include <optional>
struct Node {
    Shape* shape;
    Node* next;
    Node(Shape* shape);
    void destroy();
};
inline const unsigned int defaultVertexSize = 1000;
inline const unsigned int defaultIndexSize = 1000;
inline const unsigned int defaultMaxNewShapesPerFrame = 0; // 0 = no max
class Batch {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        std::optional<Node> firstShape;
        std::optional<Node> lastShape;
        std::optional<Node> firstShapeTBA;
        std::optional<Node> lastShapeTBA;
        bool isStatic;
        unsigned int numIndices;
        unsigned int numVertices;
        unsigned int singleSize;
        void editVertexBuffer(unsigned int offset, float* vertices, unsigned int sizeo);
        void editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo);
        Node popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices);
        unsigned int maxNewShapesPerFrame;
        void init(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame);
    public:
        Batch(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame);
        Batch(std::vector<unsigned int>, bool isStatic);
        void draw();
        void update();
        void addShapeToQueue(Shape* shape);
        void end();
        bool marked;
};
#endif
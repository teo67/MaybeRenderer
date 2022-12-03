struct Attribute {
    GLenum type;
    unsigned int num;
    unsigned int size;
};
class Batch {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        unsigned int singleSize;
    public:
        Batch(unsigned int numSizes, Attribute* sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic);
        void editVertexBuffer(unsigned int offset, float* vertices, unsigned int sizeo);
        void editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo);
        void draw(unsigned int offset, unsigned int numIndices);
        void unbindVAO();
        void bindVAO();
        void end();
};
class Batch {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        int numIndices;
        void initialize(unsigned int numSizes, unsigned int* sizes, unsigned int totalSize, unsigned int vertexSize, unsigned int indexSize, GLenum drawType);
    public:
        Batch(Shader* sampleShader, unsigned int vertexSize, unsigned int indexSize, GLenum drawType);
        Batch(unsigned int numSizes, unsigned int* sizes, unsigned int totalSize, unsigned int vertexSize, unsigned int indexSize, GLenum drawType);
        void editVertexBuffer(unsigned int offset, float* vertices);
        void editIndexBuffer(unsigned int offset, unsigned int* indices);
        void draw();
        void unbindVAO();
};
#include <iostream>
#include <glad/glad.h>
#include "Batch.h"
#include "Shader.h"
void Batch::initialize(unsigned int numSizes, unsigned int* sizes, unsigned int totalSize, unsigned int vertexSize, unsigned int indexSize, GLenum drawType) {
    this->numIndices = 0;
    glGenVertexArrays(1, &this->VAO);  
    glGenBuffers(1, &this->EBO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, (void *)(0), drawType);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, (void *)(0), drawType);
    unsigned int countSize = 0;
    for(int i = 0; i < numSizes; i++) {
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, totalSize * sizeof(float), (void*)countSize);
        glEnableVertexAttribArray(i);
        countSize += sizes[i];
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Batch::editVertexBuffer(unsigned int offset, float* vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Batch::editIndexBuffer(unsigned int offset, unsigned int* indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(indices), indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
Batch::Batch(Shader* sampleShader, unsigned int vertexSize, unsigned int indexSize, GLenum drawType) {
    unsigned int numSizes = sampleShader->getNumSizes();
    if(numSizes == -1) {
        std::cout << "Custom shaders cannot be used as sample shaders when constructing batches!" << std::endl;
        return;
    }
    initialize(numSizes, sampleShader->getSizes(), sampleShader->getTotalSize(), vertexSize, indexSize, drawType);
}
Batch::Batch(unsigned int numSizes, unsigned int* sizes, unsigned int totalSize, unsigned int vertexSize, unsigned int indexSize, GLenum drawType) {
    initialize(numSizes, sizes, totalSize, vertexSize, indexSize, drawType);
}
void Batch::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, 0);
    // don't unbind, leave for user
}
void Batch::unbindVAO() {
    glBindVertexArray(0);
}
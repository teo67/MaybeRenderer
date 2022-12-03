#include <iostream>
#include <string>
#include <cstring>
#include <glad/glad.h>
#include <vector>
#include "Batch.h"
Batch::Batch(unsigned int numSizes, Attribute* sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic) {
    glGenVertexArrays(1, &this->VAO);  
    glGenBuffers(1, &this->EBO);
    glGenBuffers(1, &this->VBO);
    singleSize = 0;
    for(int i = 0; i < numSizes; i++) {
        singleSize += sizes[i].num * sizes[i].size;
    }
    unsigned int sizeOfIndexBuffer = sizeof(unsigned int) * indexSize;
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, singleSize * vertexSize, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexBuffer, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    
    unsigned int sizeUntil = 0;
    for(int i = 0; i < numSizes; i++) {
        //std::cout << std::to_string(i) << " " << std::to_string(sizes[i].num) << " " << std::to_string(singleSize) << " " << std::to_string((intptr_t)(sizeUntil)) << std::endl;
        glVertexAttribPointer(i, sizes[i].num, sizes[i].type, GL_FALSE, singleSize, (void*)(intptr_t)(sizeUntil));
        glEnableVertexAttribArray(i);
        sizeUntil += sizes[i].num * sizes[i].size;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}
void Batch::editVertexBuffer(unsigned int offset, float* vertices, unsigned int sizeo) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeo, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Batch::editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeo, indices);
}
void Batch::draw(unsigned int offset, unsigned int numIndices) {
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)(intptr_t)offset);
}
void Batch::bindVAO() {
    glBindVertexArray(VAO);
}
void Batch::unbindVAO() {
    glBindVertexArray(0);
}
void Batch::end() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
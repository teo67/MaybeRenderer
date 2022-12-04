#include <iostream>
#include <string>
#include <cstring>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shapes/Shape.h"
#include "Batch.h"
#include "Debugger.h"
Node::Node(Shape* shape) {
    this->shape = shape;
    this->next = nullptr;
}
void Node::destroy() {
    if(next != nullptr) {
        next->destroy();
        delete next;
    }
}
Batch::Batch(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame) {
    init(sizes, vertexSize, indexSize, isStatic, maxNewShapesPerFrame);
}
Batch::Batch(std::vector<unsigned int> sizes, bool isStatic) {
    init(sizes, defaultVertexSize, defaultIndexSize, isStatic, defaultMaxNewShapesPerFrame);
}
void Batch::init(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame) {
    this->isStatic = isStatic;
    this->marked = false;
    this->numIndices = 0;
    this->numVertices = 0;
    glGenVertexArrays(1, &this->VAO);  
    glGenBuffers(1, &this->EBO);
    glGenBuffers(1, &this->VBO);
    singleSize = 0;
    for(int i = 0; i < sizes.size(); i++) {
        singleSize += sizes[i];
    }
    unsigned int sizeOfIndexBuffer = sizeof(unsigned int) * indexSize;
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, singleSize * sizeof(float) * vertexSize, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexBuffer, nullptr, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    
    unsigned int sizeUntil = 0;
    for(int i = 0; i < sizes.size(); i++) {
        //std::cout << std::to_string(i) << " " << std::to_string(sizes[i].num) << " " << std::to_string(singleSizeBytes) << " " << std::to_string((intptr_t)(sizeUntil)) << std::endl;
        glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, singleSize * sizeof(float), (void*)(intptr_t)(sizeUntil));
        glEnableVertexAttribArray(i);
        sizeUntil += sizes[i] * sizeof(float);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    this->firstShape = std::nullopt;
    this->lastShape = std::nullopt;
    this->firstShapeTBA = std::nullopt;
    this->lastShapeTBA = std::nullopt;
}
void Batch::editVertexBuffer(unsigned int offset, float* vertices, unsigned int sizeo) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //std::cout << sizeo + 1 << std::endl;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeo, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Batch::editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //std::cout << sizeo + 1 << std::endl;
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeo, indices);
}
void Batch::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
}
void Batch::end() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void Batch::addShapeToQueue(Shape* shape) {
    Node newNode(shape);
    if(!lastShapeTBA.has_value()) {
        firstShapeTBA = newNode;
        lastShapeTBA = newNode;
    } else {
        std::cout << &lastShapeTBA << std::endl;
        lastShapeTBA.value().next = &newNode;
        lastShapeTBA = newNode;
    }
}
Node Batch::popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices) {
    //Debugger::print("popping queue");
    Node returning = firstShapeTBA.value();
    numPoppedVertices += returning.shape->getNumVertices();
    numPoppedIndices += returning.shape->getNumIndices();
    if(returning.next == nullptr) {
        firstShapeTBA = std::nullopt;
        lastShapeTBA = std::nullopt;
    } else {
        firstShapeTBA = *returning.next;
    }
    //Debugger::print("popped");
    return returning;
}
void Batch::update() {
    std::optional<Node> firstPopped = std::nullopt;
    unsigned int totalPoppedVertices = 0;
    unsigned int totalPoppedIndices = 0;
    if(firstShapeTBA.has_value()) {
        unsigned int poppedCount = 1;
        Node last = popQueue(totalPoppedVertices, totalPoppedIndices);
        firstPopped = last;
        while(firstShapeTBA.has_value() && poppedCount != maxNewShapesPerFrame) {
            last = popQueue(totalPoppedVertices, totalPoppedIndices);
            poppedCount++;
        }
        last.next = nullptr;
        if(!firstShape.has_value()) {
            firstShape = firstPopped;
        } else {
            lastShape.value().next = &firstPopped.value();
        }
        lastShape = last;
        //Debugger::print("popped " + std::to_string(poppedCount) + " shape(s)");
    } // copy from queue to start of shape linked list
    unsigned int numRemovedVertices = 0;
    unsigned int numRemovedIndices = 0;
    
    if(marked || firstPopped.has_value() || !isStatic) {
        float* newData = new float[(numVertices + totalPoppedVertices) * singleSize]; // max possible len
        std::optional<Node> current = firstShape;
        unsigned int index = 0;
        while(current.has_value()) {
            Shape* cshape = current.value().shape;
            cshape->appendVertexData(newData, index);
            index += cshape->getNumVertices() * singleSize;
            if(current.value().next != nullptr && (current.value().next)->shape->getState() == ShapeState::DISABLED_PERMANENT) {
                Node* saved = current.value().next;
                current.value().next = saved->next; // remove any permanent disables
                numRemovedVertices += saved->shape->getNumVertices();
                numRemovedIndices += saved->shape->getNumIndices();
                delete saved;
            }
            if(current.value().next == nullptr) {
                break;
            }
            current = *current.value().next;
        }
        editVertexBuffer(0, newData, (numVertices + totalPoppedVertices - numRemovedVertices) * singleSize * sizeof(float));
        marked = false;
    }
    if(numRemovedVertices > 0 || firstPopped.has_value()) {
        unsigned int* newIndexData = new unsigned int[numIndices + totalPoppedIndices - numRemovedIndices];
        std::optional<Node> current = numRemovedVertices > 0 ? firstShape : firstPopped;
        unsigned int index = numRemovedVertices > 0 ? 0 : numIndices;
        unsigned int referenceIndex = numRemovedVertices > 0 ? 0 : numVertices - numRemovedVertices;
        while(current.has_value()) {
            Shape* cshape = current.value().shape;
            cshape->appendIndexData(newIndexData, index, referenceIndex);
            index += cshape->getNumIndices();
            referenceIndex += cshape->getNumVertices();
            if(current.value().next == nullptr) {
                break;
            }
            current = *current.value().next;
        }
        editIndexBuffer(0, newIndexData, (numIndices + totalPoppedIndices - numRemovedIndices) * sizeof(unsigned int));
        numIndices = index;
        numVertices = numVertices + totalPoppedVertices - numRemovedVertices;
    }
}
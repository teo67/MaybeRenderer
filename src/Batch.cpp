#include <memory>
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

Node::Node(Shape& _shape) : shape(_shape) {
    //Debugger::print("creating node");
    next = nullptr;
    //Debugger::print("node created");
}

Batch::Batch() {
    initialized = false;
    isStatic = true;
    marked = false;
    numIndices = 0;
    numVertices = 0;
    VAO = 0;
    VBO = 0;
    EBO = 0;
    singleSize = 0;
    firstShape = nullptr;
    lastShape = nullptr;
    firstShapeTBA = nullptr;
    lastShapeTBA = nullptr;
    maxNewShapesPerFrame = 0;
}
void Batch::init(std::vector<unsigned int> sizes, unsigned int vertexSize, unsigned int indexSize, bool isStatic, unsigned int maxNewShapesPerFrame) {
    initialized = true;
    this->maxNewShapesPerFrame = maxNewShapesPerFrame;
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
    //Debugger::print("initializing all nullptrs");
    firstShape = nullptr;
    lastShape = nullptr;
    firstShapeTBA = nullptr;
    lastShapeTBA = nullptr;
    //Debugger::print("initialized");
}
void Batch::init(std::vector<unsigned int> sizes, bool isStatic) {
    init(sizes, defaultVertexSize, defaultIndexSize, isStatic, defaultMaxNewShapesPerFrame);
}
void Batch::editVertexBuffer(unsigned int offset, std::vector<float>& vertices, unsigned int sizeo) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeo, static_cast<void *>(&vertices[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Batch::editIndexBuffer(unsigned int offset, unsigned int* indices, unsigned int sizeo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
void Batch::printVector(std::vector<float>& input) {
    for(int i = 0; i < input.size(); i++) {
        std::cout << input[i] << std::endl;
    }
}
void Batch::addShapeToQueue(Shape& shape) {
    //Debugger::print("making shared");
    std::shared_ptr<Node> newNode = std::make_shared<Node>(shape);
    //Debugger::print("made shared");
    if(firstShapeTBA == nullptr) {
        //Debugger::print("Assign 1");
        firstShapeTBA = newNode;
        //Debugger::print("Assign 1 complete");
    } else {
        //std::cout << &lastShapeTBA << std::endl;
        //Debugger::print("Assign 2");
        lastShapeTBA->next = newNode;
        //Debugger::print("Assign 2 complete");
    }
    //Debugger::print("Assign 3");
    lastShapeTBA = newNode;
    //Debugger::print("Assign 3 complete");
}
std::shared_ptr<Node> Batch::popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices) {
    //Debugger::print("popping queue");
    std::shared_ptr<Node> returning = firstShapeTBA;
    numPoppedVertices += returning->shape.getNumVertices();
    numPoppedIndices += returning->shape.getNumIndices();
    //Debugger::print("popped");
    if(returning->next == nullptr) {
        //Debugger::print("setting to null");
        firstShapeTBA = nullptr;
        lastShapeTBA = nullptr;
        //Debugger::print("set");
    } else {
        //Debugger::print("Assign 4");
        firstShapeTBA = returning->next;
        //Debugger::print("Assign 4 complete");
    }
    ////Debugger::print("popped");
    return returning;
}
void Batch::update() {
    std::shared_ptr<Node> firstPopped;
    unsigned int totalPoppedVertices = 0;
    unsigned int totalPoppedIndices = 0;
    if(firstShapeTBA != nullptr) {
        unsigned int poppedCount = 1;
        std::shared_ptr<Node> last = popQueue(totalPoppedVertices, totalPoppedIndices);
        //Debugger::print("5");
        firstPopped = last;
        //Debugger::print("5 complete");
        while(firstShapeTBA != nullptr && poppedCount != maxNewShapesPerFrame) {
            //Debugger::print("inside while 1");
            last = popQueue(totalPoppedVertices, totalPoppedIndices);
            poppedCount++;
        }
        //Debugger::print("completed while 1, setting 6");
        last->next = nullptr;
        //Debugger::print("6 set");
        if(firstShape == nullptr) {
            firstShape = firstPopped;
        } else {
            lastShape->next = firstPopped;
        }
        lastShape = last;
        //Debugger::print("popped " + std::to_string(poppedCount) + " shape(s)");
    } // copy from queue to start of shape linked list
    unsigned int numRemovedVertices = 0;
    unsigned int numRemovedIndices = 0;
    //Debugger::print("made it past popping");
    if(marked || totalPoppedVertices > 0 || !isStatic) {
        std::vector<float> newData((numVertices + totalPoppedVertices) * singleSize); //new float[(numVertices + totalPoppedVertices) * singleSize]; // max possible len
        std::shared_ptr<Node> current = firstShape;
        unsigned int index = 0;
        while(true) {
            Shape& cshape = current->shape;
            cshape.appendVertexData(newData, index);
            index += cshape.getNumVertices() * singleSize;
            if(current->next != nullptr && (current->next)->shape.getState() == ShapeState::DISABLED_PERMANENT) {
                numRemovedVertices += current->next->shape.getNumVertices();
                numRemovedIndices += current->next->shape.getNumIndices();
                current->next = current->next->next; // remove any permanent disables
            }
            if(current->next == nullptr) {
                break;
            }
            current = current->next;
        }
        //Debugger::print("about to edit vertex buffer");
        editVertexBuffer(0, newData, (numVertices + totalPoppedVertices - numRemovedVertices) * singleSize * sizeof(float));
        marked = false;
    }
    if(numRemovedVertices > 0 || totalPoppedVertices > 0) {
        // std::cout << "num indices: " << numIndices << std::endl;
        // std::cout << "total popped indices: " << totalPoppedIndices << std::endl;
        // std::cout << "removed indices: " << numRemovedIndices << std::endl;
        unsigned int* newIndexData = new unsigned int[numIndices + totalPoppedIndices - numRemovedIndices];
        std::shared_ptr<Node> current = numRemovedVertices > 0 ? firstShape : firstPopped;
        unsigned int index = numRemovedVertices > 0 ? 0 : numIndices;
        unsigned int referenceIndex = numRemovedVertices > 0 ? 0 : numVertices - numRemovedVertices;
        while(true) {
            Shape& cshape = current->shape;
            cshape.appendIndexData(newIndexData, index, referenceIndex);
            index += cshape.getNumIndices();
            referenceIndex += cshape.getNumVertices();
            if(current->next == nullptr) {
                break;
            }
            current = current->next;
        }
        editIndexBuffer(0, newIndexData, (numIndices + totalPoppedIndices - numRemovedIndices) * sizeof(unsigned int));
        numIndices = index;
        numVertices = numVertices + totalPoppedVertices - numRemovedVertices;
    }
}
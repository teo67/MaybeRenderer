#include <memory>
#include <iostream>
#include <string>
#include <cstring>
#include <glad/glad.h>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shapes/Shape.h"
#include "Batch.h"
#include "Debugger.h"
#include "TextureSet.h"
Batch::Batch() : textureSet() {
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
    //std::cout << "VERTICES" << std::endl;
    //printVector<float>(vertices);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeo, static_cast<void *>(&vertices[0]));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Batch::editIndexBuffer(unsigned int offset, std::vector<unsigned int>& indices, unsigned int sizeo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //std::cout << "INDICES" << std::endl;
    //printVector<unsigned int>(indices);
    //std::cout << "INDEX OFFSET: " << offset << std::endl;
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeo, static_cast<void *>(&indices[0]));
    //glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, )
}
void Batch::draw() {
    //std::cout << glGetString(GL_VERSION) << std::endl;
    glBindVertexArray(VAO);
    unsigned int size = textureSet.maps.size();
    //std::cout << "size" << size << std::endl;
    if(size > 0) {
        unsigned int totalSize = 0;
        for(unsigned int i = 0; i < size; i++) {
            std::map<unsigned int, unsigned int>& element = textureSet.maps[i];
            std::map<unsigned int, unsigned int>::iterator it;
            // for(it = element.begin(); it != element.end(); it++) {
            //     glBindTextureUnit(it->second, it->first);
            //     std::cout << "second: " << it->second << ", first: " << it->first << std::endl;
            // }
            glBindTextureUnit(0, 2);
            glBindTextureUnit(1, 1);
            //std::cout << "out of the loop" << std::endl;
            unsigned int newSize = (i == size - 1) ? numIndices : textureSet.lastIndices[i];
            glDrawElements(GL_TRIANGLES, newSize - totalSize, GL_UNSIGNED_INT, (void*)(intptr_t)totalSize);
            //std::cout << "we drew" << std::endl;
            totalSize = newSize;
        }
    } else {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)0);
    }
}
void Batch::end() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    firstShape = nullptr;
    lastShape = nullptr;
    firstShapeTBA = nullptr;
    lastShapeTBA = nullptr;
    textureSet.reset();
}
template <typename T>
void Batch::printVector(std::vector<T>& input) {
    for(int i = 0; i < input.size(); i++) {
        std::cout << input[i] << std::endl;
    }
}
void Batch::addShapeToQueue(const std::shared_ptr<Node1>& newNode) {
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
}
std::shared_ptr<Node1> Batch::popQueue(unsigned int &numPoppedVertices, unsigned int &numPoppedIndices) {
    std::shared_ptr<Node1> returning = firstShapeTBA;
    //std::cout << returning->getShape().getNumVertices() << std::endl;
    numPoppedVertices += returning->getShape().getNumVertices();
    numPoppedIndices += returning->getShape().getNumIndices();
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
    return returning;
}
void Batch::update() {
    std::shared_ptr<Node1> firstPopped;
    unsigned int totalPoppedVertices = 0;
    unsigned int totalPoppedIndices = 0;
    if(firstShapeTBA != nullptr) {
        unsigned int poppedCount = 1;
        std::shared_ptr<Node1> last = popQueue(totalPoppedVertices, totalPoppedIndices);
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
    //Debugger::print("made it past popping");
    if(marked || totalPoppedVertices > 0 || !isStatic) {
        textureSet.reset();
        unsigned int numRemovedVertices = 0;
        std::vector<float> newData((numVertices + totalPoppedVertices) * singleSize); //new float[(numVertices + totalPoppedVertices) * singleSize]; // max possible len
        std::vector<unsigned int> newIndexData(numIndices + totalPoppedIndices);
        std::shared_ptr<Node1> current = firstShape;
        std::shared_ptr<Node1> previous = nullptr;
        unsigned int iVertex = 0;
        unsigned int iIndex = 0;
        int firstIndex = -1;
        while(current != nullptr) {
            Shape& cshape = current->getShape();
            unsigned int verts = cshape.getNumVertices();
            unsigned int indis = cshape.getNumIndices();
            if(cshape.getState() == ShapeState::DISABLED_PERMANENT) {
                if(firstIndex == -1) {
                    firstIndex = iIndex;
                }
                numRemovedVertices += verts;
                if(previous == nullptr) {
                    firstShape = current->next;
                } else {
                    previous->next = current->next;
                }
                if(current->next == nullptr) {
                    lastShape = previous;
                }
            } else {
                if(firstIndex == -1 && iVertex == numVertices - numRemovedVertices) { // if we reached the new nodes
                    firstIndex = iIndex;
                }
                cshape.appendVertexData(newData, iVertex * singleSize, textureSet, iIndex);
                if(firstIndex != -1) {
                    cshape.appendIndexData(newIndexData, iIndex - firstIndex, iVertex);
                }
                iVertex += verts;
                iIndex += indis;
                previous = current;
            }
            current = current->next;
        }
        //Debugger::print("about to edit vertex buffer");
        //std::cout << "iVertex: " << iVertex << ", numRemoved: " << numRemovedVertices << std::endl;
        editVertexBuffer(0, newData, iVertex * singleSize * sizeof(float));
        //std::cout << "first Index: " << firstIndex << ", iIndex: " << iIndex << std::endl;
        //printVector<unsigned int>(newIndexData);
        if(firstIndex != -1) {
            editIndexBuffer(firstIndex * sizeof(unsigned int), newIndexData, (iIndex - firstIndex) * sizeof(unsigned int));
        }
        numVertices = iVertex;
        numIndices = iIndex;
        //std::cout << numVertices << std::endl;
        //std::cout << numIndices << std::endl;
        marked = false;
    }
}
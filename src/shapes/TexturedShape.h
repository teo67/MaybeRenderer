#ifndef TEXTUREDSHAPE_H
#define TEXTUREDSHAPE_H
#include "Shape.h"
#include "ColorShape.h"
#include <string>
#include <glad/glad.h>
class TexturedShape : public Shape {
    private:
        GLuint texture;
    public:
        TexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void setTexture(GLuint texture);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex);
        const BatchInfo& getFormat();
};

class TextureNode : public Node1 {
    private:
        TexturedShape shape;
    public:
        TextureNode(TexturedShape& _shape);
        TexturedShape& getShape() override;
};
#endif
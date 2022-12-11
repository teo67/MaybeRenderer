#ifndef TEXTUREDSHAPE_H
#define TEXTUREDSHAPE_H
#include "Shape.h"
#include "ColorShape.h"
#include <string>
class TexturedShape : public Shape {
    private:
        unsigned int texture;
    public:
        TexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic, unsigned int texture);
        void setTexture(unsigned int texture);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex);
};

class TextureNode : public Node1 {
    private:
        TexturedShape shape;
    public:
        TextureNode(TexturedShape& _shape);
        TexturedShape& getShape() override;
};
#endif
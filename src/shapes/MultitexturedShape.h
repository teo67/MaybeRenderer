#ifndef MULTITEXTUREDSHAPE_H
#define MULTITEXTUREDSHAPE_H
#include "Shape.h"
#include "ColorShape.h"
#include <string>
#include <glad/glad.h>
#include <vector>
#include "../stuff/TextureSet.h"
class MultitexturedShape : public Shape {
    private:
        std::vector<GLuint> textures;
    public:
        MultitexturedShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic);
        void setTexture(unsigned int index, GLuint texture);
        void appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex);
        const BatchInfo& getFormat();
};

class MultitextureNode : public Node1 {
    private:
        MultitexturedShape shape;
    public:
        MultitextureNode(MultitexturedShape& _shape);
        MultitexturedShape& getShape() override;
};
#endif
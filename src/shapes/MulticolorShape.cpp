#include "ColorShape.h"
#include "MulticolorSHape.h"
#include "Shape.h"
#include "../stuff/BatchInfo.h"
#include <vector>
#include <memory>
MulticolorShape::MulticolorShape(PositionInfo _positionInfo, const VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
Shape(_positionInfo, _vertexIndexInfo, _isStatic)
{
    colors = std::vector<Color>(_vertexIndexInfo.vertices.size());
}

MulticolorShape::MulticolorShape(const VertexIndexInfo& _vertexIndexInfo, bool _isStatic) :
MulticolorShape(PositionInfo(), _vertexIndexInfo, _isStatic){}

void MulticolorShape::appendVertexData(std::vector<float>& vertexData, unsigned int index, TextureSet& textureSet, unsigned int firstIndex) {
    fillPositionInfo(vertexData, index, 7);
    for(int i = 0; i < colors.size(); i++) {
        vertexData[index + 4 + 7 * i] = colors[i].r;
        vertexData[index + 5 + 7 * i] = colors[i].g;
        vertexData[index + 6 + 7 * i] = colors[i].b;
    }
}

void MulticolorShape::setColor(unsigned int vertex, float r, float g, float b) {
    if(vertex >= getNumVertices()) {
        throw "Vertex value is too large!";
    }
    colors[vertex].r = r;
    colors[vertex].b = b;
    colors[vertex].g = g;
}

void MulticolorShape::setColor(unsigned int vertex, Color& color) {
    if(vertex >= getNumVertices()) {
        throw "Vertex value is too large!";
    }
    colors[vertex] = color;
}

Color& MulticolorShape::getColor(unsigned int vertex) {
    if(vertex >= getNumVertices()) {
        throw "Vertex value is too large!";
    }
    return colors[vertex];
}

MulticolorNode::MulticolorNode(MulticolorShape& _shape) : Node1(), shape(_shape) {
}

MulticolorShape& MulticolorNode::getShape() {
    return shape;
}
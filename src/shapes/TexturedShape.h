// #ifndef TEXTUREDSHAPE_H
// #define TEXTUREDSHAPE_H
// #include "Shape.h"
// #include "ColorShape.h"
// class TexturedShape : public Shape {
//     private:
//         unsigned int textureIndex;
//     protected:
//         void init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
//     public:
//         void setTexture(char* name, Game game);
//         void setTexture(unsigned int index);
// };
// class TexturedColorShape : public ColorShape, public TexturedShape {
//     protected:
//         void init(bool isStatic, float x, float y, float z, float pitch, float yaw, float roll, float xScale, float yScale, float zScale);
// };
// #endif
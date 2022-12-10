#ifndef GAME_H
#define GAME_H
#include "BatchManager.h"
#include "ShapeManager.h"
#include "shapes/ColorShape.h"
#include "shapes/MulticolorShape.h"
class Game {
    private:
        BatchManager batman;
    public:
        Game();
        ~Game();
        ShapeManager shaman;
        void render();
        ColorShape& generateColorShape(const VertexIndexInfo& viInfo, bool isStatic);
        MulticolorShape& generateMulticolorShape(const VertexIndexInfo& viInfo, bool isStatic);
};
#endif
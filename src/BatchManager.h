#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H
#include "Batch.h"
#include "BatchInfo.h"
#include "Shader.h"
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class BatchManager {
    private:
        Batch batches[NUM_BATCHES];
        Shader shaders[NUM_SHADERS];
        void renderPair(unsigned int index1, unsigned int index2, unsigned int shaderIndex, glm::mat4 transform);
    public:
        BatchManager();
        void addShape(const std::shared_ptr<Node1>& shape);
        void updateStatic(Shape& shape);
        void updateAll();
        void renderAll(glm::mat4 transform);
        void cleanup();
        void init();
};
#endif
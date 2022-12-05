#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H
#include "Batch.h"
#include "BatchInfo.h"
#include <optional>
class BatchManager {
    private:
        std::optional<Batch> batches[NUM_BATCHES];
    public:
        BatchManager();
        void addShape(std::shared_ptr<Shape> shape);
        void updateStatic(std::shared_ptr<Shape> shape);
        void updateAll();
        void renderAll();
        void cleanup();
};
#endif
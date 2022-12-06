#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H
#include "Batch.h"
#include "BatchInfo.h"
#include <optional>
class BatchManager {
    private:
        Batch batches[NUM_BATCHES];
    public:
        BatchManager();
        void addShape(Shape& shape);
        void updateStatic(Shape& shape);
        void updateAll();
        void renderAll();
        void cleanup();
};
#endif
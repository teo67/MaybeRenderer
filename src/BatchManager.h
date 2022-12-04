#ifndef BATCH_MANAGER_H
#define BATCH_MANAGER_H
#include "Batch.h"
class BatchManager {
    private:
        Batch** batches;
    public:
        BatchManager();
        void addShape(Shape* shape);
        void updateStatic(Shape* shape);
        void updateAll();
        void renderAll();
        void cleanup();
};
#endif
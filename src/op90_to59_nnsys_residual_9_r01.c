#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>

typedef struct TransferTask {
    u32 type;
    void *src;
    u32 dst;
    u32 size;
} TransferTask;

typedef struct TransferQueue {
    TransferTask *tasks;
    u32 capacity;
    u16 front;
    u16 end;
    u16 count;
    u16 pad;
    u32 queuedBytes;
} TransferQueue;

TransferQueue _021D84D0;
void ResetTaskQueue_(TransferQueue *queue);
void NNS_GfdInitVramTransferManager(TransferTask *pTaskArray, u32 lengthOfArray);
// @fn NNS_GfdInitVramTransferManager
void NNS_GfdInitVramTransferManager(TransferTask *pTaskArray, u32 lengthOfArray) {
    _021D84D0.tasks = pTaskArray;
    _021D84D0.capacity = lengthOfArray;
    ResetTaskQueue_(&_021D84D0);
}

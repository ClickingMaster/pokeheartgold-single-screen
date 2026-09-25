#include <nitro/types.h>

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

extern TransferQueue _021D84D0;
extern int IsVramTransferTaskQueueFull_(TransferQueue *);
extern int IsVramTransferTaskQueueEmpty_(TransferQueue *);
extern u32 GetNextIndex_(TransferQueue *, u32);
extern void DoTransfer_(TransferTask *);
extern void DC_FlushRange(void *, u32);
extern void (*_02109210[])(void *, u32, u32);
void ResetTaskQueue_(TransferQueue *);
TransferTask *NNSi_GfdGetFrontVramTransferTaskQueue(TransferQueue *);
TransferTask *NNSi_GfdGetEndVramTransferTaskQueue(TransferQueue *);
int NNSi_GfdPushVramTransferTaskQueue(TransferQueue *);
int NNSi_GfdPopVramTransferTaskQueue(TransferQueue *);
void NNS_GfdInitVramTransferManager(TransferTask *, u32);
void NNS_GfdDoVramTransfer(void);
int NNS_GfdRegisterNewVramTransferTask(u32, u32, void *, u32);
void NNSi_GfdInitLnkVramMan(void **);
typedef struct VramBlock {
    u32 address;
    u32 size;
    struct VramBlock *prev;
    struct VramBlock *next;
} VramBlock;
void NNSi_GfdInitLnkVramBlockPool(VramBlock *, u32);
int NNSi_GfdAddNewFreeBlock(VramBlock **, VramBlock **, u32, u32);
int NNSi_GfdAllocLnkVram(VramBlock **, VramBlock **, u32 *, u32);
int NNSi_GfdAllocLnkVramAligned(VramBlock **, VramBlock **, u32 *, u32, u32);

void NNS_GfdDoVramTransfer(void) {
    TransferQueue *queue = &_021D84D0;
    TransferTask *task = NNSi_GfdGetFrontVramTransferTaskQueue(queue);
    if (!NNSi_GfdPopVramTransferTaskQueue(queue)) {
        return;
    }
    do {
        DoTransfer_(task);
        queue->queuedBytes -= task->size;
        task = NNSi_GfdGetFrontVramTransferTaskQueue(queue);
    } while (NNSi_GfdPopVramTransferTaskQueue(queue));
}

int NNS_GfdRegisterNewVramTransferTask(u32 type, u32 dst, void *src, u32 size) {
    TransferQueue *queue = &_021D84D0;
    TransferTask *task;
    if (IsVramTransferTaskQueueFull_(queue)) {
        return 0;
    }
    task = NNSi_GfdGetEndVramTransferTaskQueue(queue);
    task->type = type;
    task->dst = dst;
    task->src = src;
    task->size = size;
    NNSi_GfdPushVramTransferTaskQueue(queue);
    queue->queuedBytes += task->size;
    return 1;
}

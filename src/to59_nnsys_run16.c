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

int NNSi_GfdAllocLnkVram(VramBlock **freeBlocks, VramBlock **pool, u32 *address, u32 size) {
    return NNSi_GfdAllocLnkVramAligned(freeBlocks, pool, address, size, 0);
}

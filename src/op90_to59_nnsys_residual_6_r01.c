#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct FrmHeap { u8 pad[0x24]; void *head; void *tail; } FrmHeap;
int abs(int);
u32 NNS_FndGetAllocatableSizeForFrmHeapEx(FrmHeap *h, int alignment);
// @fn NNS_FndGetAllocatableSizeForFrmHeapEx
u32 NNS_FndGetAllocatableSizeForFrmHeapEx(FrmHeap *h, int alignment) {
    void *p;
    alignment = abs(alignment);
    p = (void *)(((alignment - 1) + (u32)h->head) & ~(alignment - 1));
    if ((u32)p > (u32)h->tail) return 0;
    return (u32)h->tail - (u32)p;
}

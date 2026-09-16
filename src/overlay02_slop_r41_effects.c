#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D310(void *a0, void *a1, void *data) {
    memset(data, 0, 0xf0);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0xdc), HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(data, 8, 4, 4, (u8 *)data + 0xdc);
    ov02_0224D044(*(void **)((u8 *)a1 + 0x40), data);
    PlaySE(SEQ_SE_DP_FW088);
    *(int *)((u8 *)data + 0xec) = 0;
}

WIP_LOCAL void ov02_0224D358(void *a0, void *a1, void *data) {
    memset(data, 0, 0xf0);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0xdc), HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(data, 8, 4, 4, (u8 *)data + 0xdc);
    ov02_0224CFD8(*(void **)((u8 *)a1 + 0x3c), 0xfd, data);
    PlaySE(SEQ_SE_DP_FW088);
    *(int *)((u8 *)data + 0xec) = 0;
}

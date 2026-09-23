#include "to58_r2_ov07_mid0_private.h"
extern void *sub_02014A08(int,void *,void *);
extern void ov07_02222B30(void *,void *,void *,void *);
extern void ov07_02222BC8(void *,void *);
extern void ov07_02222BDC(void *,void *);

void *ov07_02222BE4(void *owner,void *source,int heapId);
void *ov07_02222BE4(void *owner,void *source,int heapId) {
    u8 *work = Heap_Alloc(heapId,0x624);
    memset(work,0,0x624);
    if (work == NULL) GF_AssertFail();
    *(void **)(work+0x1C)=sub_02014A08(heapId,work+0x20,work+0x320);
    if (*(void **)(work+0x1C)==NULL) GF_AssertFail();
    *(void **)(work+0x620)=owner;
    MIi_CpuClear32((u32)source,(u32 *)(work+0x20),0x300);
    MIi_CpuClear32((u32)source,(u32 *)(work+0x320),0x300);
    ov07_02222B30(work,work,ov07_02222BC8,ov07_02222BDC);
    return work;
}

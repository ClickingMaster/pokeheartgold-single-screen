#include "global.h"

extern void *Save_FashionData_Get(void *);
extern void *Heap_AllocAtEnd(u32, u32);
extern void *Save_FashionData_GetEntry(void *, u32);
extern void Save_FashionData_InsertEntries(u8, void *, void *, void *);
void *ov73_021EA2B4(void *, u32, u32);
void ov73_021EA2E0(void *);

void *ov73_021EA2B4(void *save, u32 heap, u32 size) {
    void *fashionData = Save_FashionData_Get(save);
    void *copy = Heap_AllocAtEnd(heap, size);
    void *src = Save_FashionData_GetEntry(fashionData, 0);
    MIi_CpuCopyFast(src, copy, size);
    return copy;
}

void ov73_021EA2E0(void *work) {
    void *save = *(void **)((u8 *)work + 4);
    void *fashionData = Save_FashionData_Get(save);
    Save_FashionData_InsertEntries(
        (u8)*(u32 *)((u8 *)work + 8),
        *(void **)((u8 *)work + 12),
        fashionData,
        *(void **)((u8 *)work + 16));
}

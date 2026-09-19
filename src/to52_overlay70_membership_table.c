#include "to52_overlay70_membership_table_private.h"

u8 *ov70_0223F684(int heapId) {
    u32 size;
    u32 count;
    int i;
    u16 *values;
    u8 *result = Heap_Alloc((enum HeapID)0x3D, 0x1EE);
    MIi_CpuClearFast(0, (u32 *)result, 0x1EE);
    values = GfGfxLoader_LoadFromNarc_GetSizeOut((NarcId)0x4A, 0xC, 0, (enum HeapID)heapId, 0, &size);
    count = size / 2;
    for (i = 0; (u32)i < count; i++) {
        if (values[i] < 0x1EE) {
            result[values[i]] = 1;
        }
    }
    Heap_Free(values);
    return result;
}

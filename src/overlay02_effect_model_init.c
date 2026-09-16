#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224DCB0(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    int j;
    u8 *p;
    u8 *q;
    memset(data, 0, 0xe9c);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0xe88), HEAP_ID_FIELD1, 0x20);
    Field3dModel_LoadFromFilesystem((Field3dModel *)data, NARC_a_1_3_4, 8, HEAP_ID_FIELD1);
    for (i = 0, p = (u8 *)data; i < 4; i++, p += 4) {
        *(void **)(p + 0xe68) = GfGfxLoader_LoadFromNarc(NARC_a_1_3_4, i + 4, FALSE, HEAP_ID_FIELD1, FALSE);
    }
    for (j = 0, q = (u8 *)data + 0x10; j < 0x12; j++, q += 0xcc) {
        ov02_0224DEA8((Field3dObject *)q, (Field3dModel *)data, (NNSFndAllocator *)((u8 *)data + 0xe88), (void **)((u8 *)data + 0xe68));
    }
    *(FieldSystem **)((u8 *)data + 0xe78) = fieldSystem;
    *(u16 *)((u8 *)data + 0xe98) = 0;
}

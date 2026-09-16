#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D880(void *a0, FieldSystem *fieldSystem, void *work) {
    int i;
    u8 *p;
    memset(work, 0, 0xd10);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)work + 0xcfc), HEAP_ID_FIELD1, 0x20);
    Field3dModel_LoadFromFilesystem((Field3dModel *)work, (NarcId)0x86, 8, HEAP_ID_FIELD1);
    for (i = 0, p = (u8 *)work; i < 4; i++, p += 4) {
        *(void **)(p + 0xcd0) = GfGfxLoader_LoadFromNarc((NarcId)0x86, i + 4, FALSE, HEAP_ID_FIELD1, FALSE);
    }
    for (i = 0, p = (u8 *)work + 0x10; i < 0x10; i++, p += 0xcc) {
        ov02_0224D73C((Field3dObject *)p, (Field3dModel *)work, (NNSFndAllocator *)((u8 *)work + 0xcfc), (void **)((u8 *)work + 0xcd0));
    }
    *(FieldSystem **)((u8 *)work + 0xce0) = fieldSystem;
    PlayerAvatar_CopyPositionVector(*(PlayerAvatar **)((u8 *)*(void **)((u8 *)work + 0xce0) + 0x40), (VecFx32 *)((u8 *)work + 0xcf0));
    *(int *)((u8 *)work + 0xd0c) = 0;
}

WIP_LOCAL void ov02_0224D914(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    u8 *p;
    for (i = 0, p = (u8 *)data + 0x10; i < 0x10; i++, p += 0xcc) {
        ov02_0224D788(p, (NNSFndAllocator *)((u8 *)data + 0xCFC));
    }
    Field3dModel_Unload((Field3dModel *)data);
    for (i = 0; i < 4; i++) {
        Heap_Free(*(void **)((u8 *)data + 0xCD0));
        data = (u8 *)data + 4;
    }
}

WIP_LOCAL void ov02_0224D950(void *a0, void *a1, void *data) {
    if (*(u32 *)((u8 *)data + 0xD0C) == 0) {
        *(int *)((u8 *)data + 0xCE4) = *(int *)((u8 *)data + 0xCE4) - 1;
        if (*(int *)((u8 *)data + 0xCE4) < 0) {
            *(int *)((u8 *)data + 0xCE4) = 4;
            ov02_0224D820(data);
        }
        ov02_0224D868(data);
    }
}

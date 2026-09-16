#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D0C8(void *data, int a1, int a2, int a3, void *a4) {
    int i;
    u8 *anim;
    memset(data, 0, 0xdc);
    Field3dModel_LoadFromFilesystem((Field3dModel *)((u8 *)data + 0x78), NARC_a_1_3_4, a1, HEAP_ID_FIELD1);
    Field3dObject_InitFromModel((Field3dObject *)data, (Field3dModel *)((u8 *)data + 0x78));
    *(int *)((u8 *)data + 0xd8) = a3;
    i = 0;
    if ((u32)i < *(u32 *)((u8 *)data + 0xd8)) {
        anim = (u8 *)data + 0x88;
        do {
            Field3dModelAnimation_LoadFromFilesystem((Field3DModelAnimation *)anim, (Field3dModel *)((u8 *)data + 0x78), NARC_a_1_3_4, a2 + i, HEAP_ID_FIELD1, a4);
            Field3dObject_AddAnimation((Field3dObject *)data, (Field3DModelAnimation *)anim);
            i++;
            anim += 0x14;
        } while (i < *(u32 *)((u8 *)data + 0xd8));
    }
}

WIP_LOCAL void ov02_0224D144(void *obj, void *alloc) {
    u32 i;
    Field3dModel_Unload((Field3dModel *)((u8 *)obj + 0x78));
    for (i = 0; i < *(u32 *)((u8 *)obj + 0xd8); i++) {
        Field3dModelAnimation_Unload((Field3DModelAnimation *)((u8 *)obj + 0x88 + i * 0x14), (NNSFndAllocator *)alloc);
    }
}

WIP_LOCAL BOOL ov02_0224D178(void *obj) {
    BOOL result = TRUE;
    u32 i;
    for (i = 0; i < *(u32 *)((u8 *)obj + 0xd8); i++) {
        result &= Field3dModelAnimation_FrameAdvanceAndCheck((Field3DModelAnimation *)((u8 *)obj + 0x88 + i * 0x14), FX32_ONE);
    }
    return result;
}

WIP_LOCAL void ov02_0224D1AC(void *data) {
    u32 i;
    for (i = 0; i < *(u32 *)((u8 *)data + 0xd8); i++) {
        Field3dModelAnimation_FrameAdvanceAndLoop((Field3DModelAnimation *)((u8 *)data + 0x88 + i * 0x14), FX32_ONE);
    }
}

#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224DD4C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    u8 *p;
    for (i = 0, p = (u8 *)data + 0x10; i < 0x12; i++, p += 0xcc) {
        ov02_0224DEF4(p, (NNSFndAllocator *)((u8 *)data + 0xE88));
    }
    Field3dModel_Unload((Field3dModel *)data);
    for (i = 0; i < 4; i++) {
        Heap_Free(*(void **)((u8 *)data + 0xE68));
        data = (u8 *)data + 4;
    }
}

WIP_LOCAL void ov02_0224DD8C(void *a0, void *a1, void *data) {
    if (*(u16 *)((u8 *)data + 0xE98) == 0) {
        *(int *)((u8 *)data + 0xE7C) = *(int *)((u8 *)data + 0xE7C) - 1;
        if (*(int *)((u8 *)data + 0xE7C) < 0) {
            *(int *)((u8 *)data + 0xE7C) = 4;
            ov02_0224DF1C(data);
        }
        ov02_0224E008(data);
    }
}

WIP_LOCAL void ov02_0224DDC8(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    u8 *p;
    for (i = 0, p = (u8 *)data + 0x10; i < 0x12; i++, p += 0xcc) {
        Field3dObject_Draw((Field3dObject *)p);
    }
}

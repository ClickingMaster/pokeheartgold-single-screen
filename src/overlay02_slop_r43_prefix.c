#include "overlay02_slop_r3_private.h"

WIP_LOCAL void ov02_0224D43C(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    memset(data, 0, 0x1cc);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0x1b8), HEAP_ID_FIELD1, 0x20);
    *(u16 *)((u8 *)data + 0x1ca) = 0;
}

WIP_LOCAL void ov02_0224D468(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    int i;
    u8 *p = data;
    u8 *q;
    for (i = 0, q = (u8 *)data + 0x1b8; i < 2; i++, p += 0xdc) {
        ov02_0224D144(p, q);
    }
}

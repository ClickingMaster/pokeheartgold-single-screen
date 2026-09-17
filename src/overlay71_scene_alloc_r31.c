#include "global.h"
void *Heap_Alloc(u32, u32);
void Main_SetVBlankIntrCB(void *, void *);
void *ov71_02247384(void *);
void ov71_0224A5A8(void *);
void ov71_0224A8B0(void *);
void *ov71_0224A0F0(void *owner);
void *ov71_0224A0F0(void *owner) {
    u8 *work = Heap_Alloc(0x39, 0xB0);
    if (work != NULL) {
        *(void **)(work + 0) = owner;
        *(u32 *)(work + 4) = 0;
        *(void **)(work + 0x24) = ov71_02247384(owner);
        *(u32 *)(work + 0x28) = 0;
        *(u32 *)(work + 0x0C) = 0;
        ov71_0224A5A8(work + 0x34);
        Main_SetVBlankIntrCB(ov71_0224A8B0, work);
    }
    return work;
}

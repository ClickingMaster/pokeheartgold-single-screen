#include "field_overlay_effects_internal.h"
void ov01_021EFD3C(SysTask *, void *);
void ov01_021EFD3C(SysTask *task, void *arg) {
    u8 *p = arg;
    switch (*(u32 *)p) {
    case 0:
        if (*(u32 *)(p + 0xC) == 1) {
            ov01_021EFFB0((Interp *)(p + 0x30), 0, *(s32 *)(p + 0x14), 2, 8);
        } else if (*(u32 *)(p + 0xC) == 2) {
            ov01_021EFFB0((Interp *)(p + 0x30), 0, *(s32 *)(p + 0x14), 1, 8);
        }
        ++*(u32 *)p;
        break;
    case 1:
        ov01_021EFFB0((Interp *)(p + 0x18), 0, *(s32 *)(p + 0x10), *(s32 *)(p + 0xC), 3);
        ++*(u32 *)p;
        break;
    case 2:
        if (ov01_021EFFBC((Interp *)(p + 0x18))) {
            ++*(u32 *)p;
        }
        break;
    case 3:
        ov01_021EFFB0((Interp *)(p + 0x18), *(s32 *)(p + 0x10), 0, *(s32 *)(p + 0xC), 3);
        ++*(u32 *)p;
        break;
    case 4:
        if (ov01_021EFFBC((Interp *)(p + 0x18))) {
            ++*(u32 *)(p + 8);
            if (*(u32 *)(p + 8) == *(u32 *)(p + 4)) {
                *(u32 *)p = 5;
            } else {
                *(u32 *)p = 1;
            }
        }
        break;
    case 5:
        *(u32 *)p = 0;
        *(u32 *)(p + 8) = 0;
        if (*(void **)(p + 0x48) != NULL) {
            **(u32 **)(p + 0x48) = 1;
        }
        SysTask_Destroy(task);
        Heap_Free(p);
        return;
    }
    ov01_021EFFBC((Interp *)(p + 0x30));
}

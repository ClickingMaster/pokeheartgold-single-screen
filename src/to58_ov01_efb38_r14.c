#include "field_overlay_effects_internal.h"
void HBlankTask_Delete(void *);
void ov01_021F0CDC(void *arg) {
    u8 *p = arg;
    *(volatile u32 *)0x04000000 &= 0xFFFF1FFF;
    **(u32 **)(p + 0xE4) = 1;
    HBlankTask_Delete(*(void **)(p + 0xDC));
    *(void **)(p + 0xDC) = NULL;
    SysTask_Destroy(*(SysTask **)(p + 0xE0));
    *(void **)(p + 0xE0) = NULL;
}


int ov01_021F0D20(void *);
int ov01_021F0D20(void *arg) {
    u8 *p = arg;
    u32 flags = *(u32 *)p;
    int active = 1;
    int mode;
    if ((flags & 1) == 0) {
        if ((flags & 0x1720) != 0 || flags == 0) {
            active = 0;
        } else {
            GF_AssertFail();
            active = 0;
        }
    }
    switch (*(u32 *)(p + 0x150)) {
    case 0: case 1: case 2: case 3: case 4: case 6: case 8: case 9: case 10: case 11: case 24:
        mode = 0;
        break;
    case 7:
        mode = 2;
        break;
    case 5:
        mode = 4;
        break;
    }
    if (*(u32 *)(p + 0x15C) - 3 <= 1) {
        mode++;
    }
    return mode + 6 * active;
}

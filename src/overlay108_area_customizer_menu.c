#include "overlay108_area_customizer_menu_private.h"
typedef struct MenuBits {
    u8 pad[0x27];
    u8 flag : 1;
    u8 selected : 7;
} MenuBits;
u32 ov108_021E5DB8(Ov108Work2 *work) {
    s32 input = TouchscreenListMenu_HandleInput(work->menu);
    u32 selected;
    if (input == -1) {
        return 1;
    }
    selected = ((MenuBits *)work->menu)->selected;
    TouchscreenListMenu_Destroy(work->menu);
    if (selected != work->unk10) {
        if (selected == 0) {
            ov108_021E61E8(work);
        } else {
            ov108_021E6238(work);
        }
    }
    work->unk10 = selected;
    switch (input) {
    case 0:
        work->selectedB = 0;
        return 2;
    case 1:
        ov108_021E79A8(work, 1, 0, 0);
        ov108_021E79A8(work, 4, 0, 0);
        work->selectedB = work->selectedA;
        ov108_021E6804(work);
        work->unk0C = 4;
        return 4;
    case 2:
    default:
        ov108_021E79A8(work, 1, 0, 0);
        ov108_021E79A8(work, 4, 0, 0);
        return 0;
    }
}

u32 ov108_021E5E68(Ov108Work2 *work) {
    switch (work->unk08) {
    case 0:
        ov108_021E7F7C(work);
        work->unk08++;
        break;
    case 1:
        if (ov108_021E80F4(work)) {
            work->unk08++;
        }
        break;
    default:
        *(volatile u32 *)0x04001000 &= 0xFFFF1FFF;
        work->unk184E1 = 0;
        work->unk08 = 0;
        if (work->flags.high == 0) {
            work->unk0C = 3;
            ov108_021E7700(work, 0, 1, 1);
            ov108_021E7EB0(work);
            work->flags.high = 1;
        } else {
            work->unk0C = 2;
            ov108_021E7700(work, 0, 0, 1);
            ov108_021E7CD8(work, work->selectedA);
            work->flags.high = 0;
        }
        return 4;
    }
    return 2;
}

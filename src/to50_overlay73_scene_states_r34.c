#include "to50_overlay73_scene_states_r34_private.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
int ov73_021E9D6C(void *work) {
    int value = 0;
    switch (U32(work, 0xF1C) + 7) {
    case 1:
    case 5:
        value = 13;
        break;
    case 8:
        value = 11;
        break;
    case 6:
    case 9:
        value = 12;
        break;
    case 0:
    case 3:
        value = 14;
        break;
    case 4:
        value = 15;
        break;
    }
    ov73_021EA180(work);
    ov73_021E9FF8(work, (void *)U32(work, 0xBD4), value, 1, 0xF0F);
    ov73_021E83EC(work, 0x2F, 0x36);
    return 3;
}
int ov73_021E9DE0(void *work) {
    switch (S16(work, 0xF90)) {
    case 0:
        ov73_021E9FF8(work, (void *)U32(work, 0xBD4), 0x10, 1, 0xF0F);
        S16(work, 0xF90)
        ++;
        break;
    case 1:
        if (!TextPrinterCheckActive((u8)U32(work, 0xBF4))) {
            sub_0203946C();
            ov00_021EC8D8();
            S16(work, 0xF90)
            ++;
        }
        break;
    case 2:
        ov73_021E9FF8(work, (void *)U32(work, 0xBD4), 0x11, 1, 0xF0F);
        S16(work, 0xF90)
        ++;
        break;
    case 3:
        if (!TextPrinterCheckActive((u8)U32(work, 0xBF4))) {
            S16(work, 0xF90)
            ++;
        }
        break;
    default:
        S16(work, 0xF92)
        ++;
        if (S16(work, 0xF92) > 30) {
            ov73_021E844C(work, 0, 0);
            U32(work, 0x1C) = 0x2E;
        }
        break;
    }
    return 3;
}

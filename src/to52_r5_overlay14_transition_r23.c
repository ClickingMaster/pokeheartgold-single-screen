#include "to52_r5_overlay14_transition_r23_private.h"

void ov14_021EBDE8(void *work) {
    *(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x40c0) = ov14_021E6070(work, *((u8 *)work + 0x21), 0xb, 0);
    ov14_021E7DF8(work);
    ov14_021F638C(*(void **)((u8 *)work + 0x34));
    ov14_021E7E40(*(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x2f0));
    ov14_021F0234(work, ov14_021E94A8, 0x15);
}

u32 ov14_021EBE2C(void *work) {
    ov14_021F6AC0(work, 8, 0);
    ov14_021F3488(work, 1, 1);
    ov14_021F3488(work, 2, 1);
    ov14_021F34C8(*(void **)((u8 *)work + 0x34), *((u8 *)work + 0x21), 0);
    ov14_021F67A4(work, 0x26);
    return 0x16;
}

u32 ov14_021EBE68(void *work) {
    u32 input = GridInputHandler_HandleInput_AllowHold(*(void **)((u8 *)*(void **)((u8 *)work + 0x34) + 0x2c));
    switch (input) {
    case 0:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 0);
        break;
    case 1:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 1);
        break;
    case 2:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 2);
        break;
    case 3:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 3);
        break;
    case 4:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 4);
        break;
    case 5:
        PlaySE(0x5dd);
        ov14_021E7E10(work, 5);
        break;
    case 6: {
        u8 value = *(u32 *)((u8 *)*(void **)((u8 *)work + 0x34) + 0x40c0);
        ov14_021E6094(work, *((u8 *)work + 0x21), 0xb, &value);
        ov14_021E895C(work, value);
        PlaySE(0x5dd);
        return ov14_021F23F0(work, 0, 0x9a);
    }
    case 7:
    case (u32)-2:
        PlaySE(0x5dc);
        return ov14_021F23F0(work, 1, 0x9a);
    case (u32)-3:
        PlaySE(0x5dc);
        return ov14_021F0244(work, 0x18);
    case (u32)-4:
        PlaySE(0x5dc);
        break;
    }
    return 0x16;
}

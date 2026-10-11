#include "overlay14_box_move_actions_r38_private.h"

u32 ov14_021F1540(void *p) {
    PCBox_SetSpriteAnim(PTR_AT(p, 0x34), 9, 8);
    if (PCStorage_CountMonsAndEggsInBox(PTR_AT(p, 4), U8_AT(p, 0x25)) == 30) {
        PlaySE(0x5f3);
    } else {
        PlaySE(0x5dd);
    }
    return PCBox_FlashPanel(p, 12, 0x66);
}

u32 ov14_021F1580(void *p, u32 value) {
    U8_AT(p, 0x21) = value;
    U8_AT(PTR_AT(p, 0x34), 0x44b) = 1;
    PCBox_SetIconLayer(PTR_AT(p, 0x34), U8_AT(p, 0x21), 0);
    PCBox_HideSelectedIconOutline(p);
    if (U8_AT(p, 0x2a) == 0) {
        ov14_021E8824(PTR_AT(p, 0x34));
    }
    return PCBox_WaitForUpdateFunc(p, ov14_021EA254, 0x73);
}

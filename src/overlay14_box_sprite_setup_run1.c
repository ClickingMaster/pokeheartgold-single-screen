#include "overlay14_box_sprite_setup_private.h"

void PCBox_MoveHeldItemIconToSlot(void *p, u32 index, u32 mode) {
    s16 pos[2];
    PCBox_GetSlotPosition(index, &pos[1], &pos[0], mode);
    PCBox_SetHeldItemIconPosition(p, (s16)(pos[1] + 8), (s16)(pos[0] + 8));
}

void ov14_021F39A0(void *p, u32 index, u32 mode) {
    s16 pos[2];
    PCBox_GetSlotPosition(index, &pos[1], &pos[0], mode);
    PCBox_SetHeldItemIconPosition(p, pos[1], (s16)(pos[0] + 4));
}

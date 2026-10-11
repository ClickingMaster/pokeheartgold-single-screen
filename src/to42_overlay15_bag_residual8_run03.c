#include "to42_overlay15_bag_residual8_private.h"

void BagApp_State_DropMovedItem(void *work) {
    u8 *state = PTR(work, 0x234);
    u8 pocket = U8_AT(state, 0x64);
    u8 *slot = state + 4 + pocket * 12;
    s16 pos = S16_AT(slot, 6);
    u16 target = pos + U32_AT(work, 0x66c);
    if (U8_AT(work, 0x672) != target) {
        MoveItemSlotInList(PTR(slot, 0), U8_AT(work, 0x672), target, pocket, 6);
        BagApp_LoadPocketItemNames(work);
        U32_AT(work, 0x644) = U32_AT(work, 0x66c) + 8;
        BagApp_SetPocketPositionFromButton(work, U32_AT(work, 0x644));
    }
    BagApp_State_EndMoveItem(work);
}

#include "to52_r4_overlay15_bag_item_use_private.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))

BOOL BagApp_TryUseItemInPlace(void *work, u16 item) {
    void *string;
    BufferPlayersName(PTR(work, 0x2f4), 0, PTR(work, 0x23c));
    BufferItemName(PTR(work, 0x2f4), 1, item);
    if (item == ITEM_BLACK_FLUTE) {
        string = NewString_ReadMsgData(PTR(work, 0x2f0), 65);
        BagApp_SetFlute(work, 1);
        U16(work, 0x680) = 0;
    } else if (item == ITEM_WHITE_FLUTE) {
        string = NewString_ReadMsgData(PTR(work, 0x2f0), 64);
        BagApp_SetFlute(work, 2);
        U16(work, 0x680) = 0;
    } else if (item == ITEM_MAX_REPEL || item == ITEM_SUPER_REPEL || item == ITEM_REPEL) {
        string = BagApp_TryUseRepel(work, item);
    } else if (item == ITEM_GB_SOUNDS) {
        string = BagApp_ToggleGBSounds(work, item);
        U16(work, 0x680) = 0;
    } else {
        return FALSE;
    }
    StringExpandPlaceholders(PTR(work, 0x2f4), PTR(work, 0x5e4), string);
    String_Delete(string);
    return TRUE;
}

void *BagApp_TryUseRepel(void *work, u16 item) {
    if (RoamerSave_RepelNotInUse(BagApp_GetSaveRoamers(work)) == FALSE) {
        U16(work, 0x680) = 0;
        return NewString_ReadMsgData(PTR(work, 0x2f0), 63);
    }
    BagApp_GetRepelStepCountAddr(work, (u8)GetItemAttr(item, 2, 6));
    U16(work, 0x680) = 1;
    PlaySE(SEQ_SE_DP_CARD2);
    return NewString_ReadMsgData(PTR(work, 0x2f0), 62);
}

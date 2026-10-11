#include "to50_overlay15_bag_quantity_branch_private.h"
s32 BagApp_ItemAction_Trash(void *work) {
    void *args;
    u8 *entry;
    U16(work, 0x680) = 1;
    BagApp_HideCursor(work);
    args = PTR(work, 0x234);
    entry = (u8 *)args + U8(args, 0x64) * 12;
    if (Pocket_GetQuantity(PTR(entry, 4), U8(entry, 0xD), U16(args, 0x66), 6) == 1) {
        BagApp_PrintTrashConfirmMessage(work);
        BagApp_ShowCancelButton(work, 0);
        return 8;
    }
    BagApp_LoadSubScreenLayout(work, 3, 0, 0);
    BagApp_ShowQuantityButtons(work, 3, U16(work, 0x682));
    BagApp_PrintQuantityDigits(work, 3);
    BagApp_PrintTrashLabel(work);
    BagApp_PrintCancelButton(work, 1);
    return 5;
}

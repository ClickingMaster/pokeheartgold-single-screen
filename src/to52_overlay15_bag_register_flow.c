#include "to52_overlay15_bag_register_flow_private.h"

#define REFRESH_SELECTION(work, args, mode)                                                     \
    do {                                                                                        \
        BagApp_HideItemMenuSprites(work);                                                                    \
        BagApp_ClearSelectedItemEntry(work);                                                                    \
        BagApp_ClearItemMenuLabels((u8 *)(work) + 0x184);                                                    \
        (args) = PTR(work, 0x234);                                                              \
        BagApp_DrawItemIcons(work, (u8 *)(args) + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), mode); \
        BagApp_ClearMoveInfoWindow(work);                                                                    \
        BagApp_CloseItemMenuWindows(work);                                                                    \
        BagApp_RedrawPocket(work);                                                                    \
    } while (0)

s32 BagApp_State_FinishTrash(void *work) {
    BagArgs *args;
    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        ClearFrameAndWindow2((u8 *)work + 0x34, TRUE);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        Pocket_TakeItem(args->entries[args->selected].slots, args->entries[args->selected].quantity, args->item, U16(work, 0x680), 6);
        BagApp_LoadPocketItemNames(work);
        args = PTR(work, 0x234);
        BagApp_DrawItemIcons(work, (u8 *)args + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), TRUE);
        BagApp_ClearMoveInfoWindow(work);
        BagApp_CloseItemMenuWindows(work);
        BagApp_RedrawPocket(work);
        BagApp_PrintCursorDescription(work);
        BagApp_ShowCancelButton(work, TRUE);
        return TRUE;
    }
    return 0xB;
}

s32 BagApp_ItemAction_Register(void *work) {
    void *args = PTR(work, 0x234);
    void *str;
    if (Bag_TryRegisterItem(PTR(work, 0x238), U16(args, 0x66)) == 0) {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x67);
        FillWindowPixelBuffer((u8 *)work + 0x34, 0xF);
        StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
        String_Delete(str);
        BagApp_PrintMessage(work, 0);
        BagApp_HideItemMenuSprites(work);
        BagApp_HideCursor(work);
        BagApp_ShowCancelButton(work, FALSE);
        return 0x19;
    }
    REFRESH_SELECTION(work, args, FALSE);
    return TRUE;
}

s32 BagApp_State_WaitRegisterFailMessage(void *work) {
    void *args;
    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        ClearFrameAndWindow2((u8 *)work + 0x34, TRUE);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        REFRESH_SELECTION(work, args, FALSE);
        BagApp_ShowCancelButton(work, TRUE);
        return TRUE;
    }
    return 0x19;
}

s32 BagApp_ItemAction_Deselect(void *work) {
    void *args;
    Bag_UnregisterItem(PTR(work, 0x238), U16(PTR(work, 0x234), 0x66));
    REFRESH_SELECTION(work, args, FALSE);
    return TRUE;
}

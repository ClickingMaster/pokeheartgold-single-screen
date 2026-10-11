#include "to42_overlay15_bag_residual8_private.h"

int BagApp_State_CloseItemMenu(void *work) {
    u8 *state;
    u8 *slot;
    int count;
    BagApp_HideItemMenuSprites(work);
    BagApp_ClearSelectedItemEntry(work);
    BagApp_ClearItemMenuLabels((u8 *)work + 0x184);
    state = PTR(work, 0x234);
    count = BagApp_GetPageItemCount(work);
    slot = state + 4 + U8_AT(state, 0x64) * 12;
    BagApp_DrawItemIcons(work, slot, count, 0);
    BagApp_ClearMoveInfoWindow(work);
    BagApp_CloseItemMenuWindows(work);
    BagApp_RedrawPocket(work);
    return 1;
}

void BagApp_State_RunItemAction(void *work) {
    void (*func)(void *);
    BagApp_HideItemMenuSprites(work);
    BagApp_ClearItemMenuLabels((u8 *)work + 0x184);
    func = *(void (**)(void *))((u8 *)work + 0x7f0 + U32_AT(work, 0x948) * 4);
    func(work);
}

int BagApp_ItemAction_Use(void *work) {
    void *func;
    int result;
    BagApp_ShowCancelButton(work, 0);
    BagApp_HideCursor(work);
    func = GetItemFieldUseFunc(2, GetItemAttr(U16_AT(PTR(work, 0x234), 0x66), 6, 6));
    if (func != NULL && (result = ((int (*)(void *))func)(PTR(PTR(work, 0x234), 0x70))) != 0) {
        GetItemUseErrorMessage(PTR(work, 0x23c), PTR(work, 0x5e4), U16_AT(PTR(work, 0x234), 0x66), result, 6);
        U8_AT(work, 0x616) = BagApp_PrintMessage(work, 0);
        return 12;
    }
    return BagApp_UseSelectedItem(work);
}

int BagApp_State_WaitItemMessage(void *work) {
    if (!TextPrinterCheckActive(U8_AT(work, 0x616))) {
        if ((gSystem.newKeys & 3) || gSystem.touchNew) {
            if (U8_AT(PTR(work, 0x234), 0x65) != 3) {
                BagApp_CloseItemMenuWindows(work);
            }
            ClearFrameAndWindow2((u8 *)work + 0x34, 1);
            ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
            ScheduleWindowCopyToVram((u8 *)work + 4);
            BagApp_ShowCancelButton(work, 1);
            BagApp_RedrawPocket(work);
            return U8_AT(PTR(work, 0x234), 0x65) == 3 ? 26 : 1;
        }
    }
    return 12;
}

int BagApp_UseSelectedItem(void *work) {
    u8 *slot = (u8 *)PTR(work, 0x234) + 4 + U8_AT(PTR(work, 0x234), 0x64) * 12;
    U8_AT(work, 0x67b) = 0;
    if (U8_AT(slot, 8) == 3) {
        PTR(work, 0x67c) = BagApp_UseTMHMTask;
        return 13;
    }
    if (TryFormatRegisteredKeyItemUseMessage(PTR(PTR(work, 0x234), 0), PTR(work, 0x5e4), U16_AT(PTR(work, 0x234), 0x66), 6) == TRUE) {
        U8_AT(work, 0x616) = BagApp_PrintMessage(work, 0);
        return 12;
    }
    if (BagApp_TryUseItemInPlace(work, U16_AT(PTR(work, 0x234), 0x66)) == TRUE) {
        PTR(work, 0x67c) = BagApp_UseItemInPlaceTask;
        return 13;
    }
    sub_020880CC(1, 6);
    U16_AT(PTR(work, 0x234), 0x68) = 0;
    return 0x25;
}

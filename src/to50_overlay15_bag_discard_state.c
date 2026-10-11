#include "to50_overlay15_bag_discard_state_private.h"
s32 BagApp_UseItemInPlaceTask(void *work) {
    void *args;
    switch (U8(work, 0x67B)) {
    case 0:
        U8(work, 0x616) = BagApp_PrintMessage(work, 0);
        U8(work, 0x67B) = 1;
        break;
    case 1:
        if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
            BagApp_CloseItemMenuWindows(work);
            ClearFrameAndWindow2((u8 *)work + 0x34, 1);
            ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
            BagApp_TakeUsedItem(work);
            BagApp_RedrawPocket(work);
            args = PTR(work, 0x234);
            BagApp_DrawItemIcons(work, (u8 *)args + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), 1);
            BagApp_PrintCursorDescription(work);
            ScheduleWindowCopyToVram((u8 *)work + 4);
            BagApp_ShowCancelButton(work, 1);
            U8(work, 0x67B) = 0;
            return 1;
        }
        break;
    }
    return 0xD;
}

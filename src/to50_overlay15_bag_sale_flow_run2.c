#include "to50_overlay15_bag_sale_flow_private.h"
s32 BagApp_State_WaitSaleMessage(void *work) {
    void *args;

    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        U32(work, 0x684) = 0;
        EraseFrameAndWindow1((u8 *)work + 0x214, 1);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        BagApp_DrawItemIcons(work, (u8 *)args + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), TRUE);
        BagApp_ClearMoveInfoWindow(work);
        BagApp_CloseItemMenuWindows(work);
        BagApp_RedrawPocket(work);
        BagApp_PrintCursorDescription(work);
        BagApp_ShowPocketTabSprites(work, 1);
        BagApp_ShowCancelButton(work, 1);
        return 0x10;
    }
    return 0x18;
}

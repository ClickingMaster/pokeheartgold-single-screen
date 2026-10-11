#include "to50_overlay15_bag_toss_confirm_private.h"
s32 BagApp_State_TrashYesNo(void *work) {
    void *args;
    void *str;
    u32 input = YesNoPrompt_HandleInput(PTR(work, 0x804));
    switch (input) {
    case 1:
        BagApp_DestroyYesNoPrompt(work);
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x36);
        if (S16(work, 0x680) == 1) {
            BufferItemName(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        } else {
            BufferItemNamePlural(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        }
        BufferIntegerAsString(PTR(work, 0x2F4), 1, S16(work, 0x680), 3, 0, 1);
        StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
        String_Delete(str);
        FillWindowPixelBuffer((u8 *)work + 0x34, 0xF);
        U8(work, 0x616) = BagApp_PrintMessage(work, 0);
        return 10;
    case 2:
        BagApp_DestroyYesNoPrompt(work);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        BagApp_DrawItemIcons(work, (u8 *)args + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), 0);
        BagApp_ClearMoveInfoWindow(work);
        BagApp_CloseItemMenuWindows(work);
        BagApp_ShowCancelButton(work, 1);
        BagApp_RedrawPocket(work);
        return 1;
    case (u32)-1:
        break;
    }
    return 9;
}

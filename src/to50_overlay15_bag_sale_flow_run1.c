#include "to50_overlay15_bag_sale_flow_private.h"

s32 BagApp_State_SellYesNo(void *work) {
    void *args;
    void *string;
    u32 input = YesNoPrompt_HandleInput(PTR(work, 0x804));

    switch (input) {
    case 1:
        BagApp_DestroyYesNoPrompt(work);
        string = NewString_ReadMsgData(PTR(work, 0x2F0), 0x4F);
        if (S16(work, 0x680) > 1) {
            BufferItemNamePlural(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        } else {
            BufferItemName(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        }
        BufferIntegerAsString(PTR(work, 0x2F4), 1, S16(work, 0x680) * U32(work, 0x684), 6, 0, 1);
        StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), string);
        String_Delete(string);
        U8(work, 0x616) = BagApp_PrintMessage(work, 0);
        return 0x17;
    case 2:
        BagApp_DestroyYesNoPrompt(work);
        U32(work, 0x684) = 0;
        EraseFrameAndWindow1((u8 *)work + 0x214, 1);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        BagApp_DrawItemIcons(work, (u8 *)args + 4 + U8(args, 0x64) * 12, BagApp_GetPageItemCount(work), FALSE);
        BagApp_ClearMoveInfoWindow(work);
        BagApp_CloseItemMenuWindows(work);
        BagApp_RedrawPocket(work);
        BagApp_ShowPocketTabSprites(work, 1);
        BagApp_ShowCancelButton(work, 1);
        return 0x10;
    case (u32)-1:
        break;
    }
    return 0x16;
}

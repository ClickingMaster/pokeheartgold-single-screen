#include "to42_overlay15_r16_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

s32 BagApp_State_ShowSellPrice(void *work) {
    void *str;
    s32 n;
    EraseFrameAndWindow1((u8 *)work + 0x44, TRUE);
    BagApp_ClearQuantityLabel(work);
    FillWindowPixelBuffer((u8 *)work + 0x34, 15);
    str = NewString_ReadMsgData(PTR(work, 0x2f0), 78);
    n = S16(work, 0x680) * U32(work, 0x684);
    BufferIntegerAsString(PTR(work, 0x2f4), 0, n, 6, 0, TRUE);
    StringExpandPlaceholders(PTR(work, 0x2f4), PTR(work, 0x5e4), str);
    String_Delete(str);
    U8(work, 0x616) = BagApp_PrintMessage(work, 1);
    BagApp_HideQuantityButtons(work);
    BagApp_HideCursor(work);
    BagApp_ShowCancelButton(work, 0);
    return 21;
}

s32 BagApp_State_CancelSellQuantity(void *work) {
    void *state;
    U32(work, 0x684) = 0;
    EraseFrameAndWindow1((u8 *)work + 0x214, TRUE);
    EraseFrameAndWindow1((u8 *)work + 0x44, TRUE);
    ClearFrameAndWindow2((u8 *)work + 0x34, TRUE);
    ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
    ScheduleWindowCopyToVram((u8 *)work + 4);
    BagApp_HideQuantityButtons(work);
    BagApp_HideCursor(work);
    state = PTR(work, 0x234);
    BagApp_DrawItemIcons(work, (u8 *)state + 4 + U8(state, 0x64) * 12, BagApp_GetPageItemCount(work), 0);
    BagApp_ClearMoveInfoWindow(work);
    BagApp_CloseItemMenuWindows(work);
    BagApp_RedrawPocket(work);
    BagApp_ShowPocketTabSprites(work, 1);
    BagApp_ShowCancelButton(work, 1);
    return 16;
}

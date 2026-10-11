#include "overlay15_bag_quantity_branch_private.h"

int BagApp_State_StartSell(BagWorkRaw *work) {
    u8 *slot = (u8 *)P(work, 0x234) + 4 + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12;
    String *str;
    s32 attr;
    u32 zero;

    BagApp_LoadSubScreenLayout(work, 4, 0, 0);
    U16(work, 0x680) = 1;
    attr = GetItemAttr(U16((BagWorkRaw *)P(work, 0x234), 0x66), 0, (enum HeapID)6);
    zero = 0;
    S32(work, 0x684) = attr >> 1;
    BagApp_ApplyTilemapCommands(P(work, 0), 5, 0);
    ScheduleBgTilemapBufferTransfer(P(work, 0), 5);
    ManagedSprite_SetDrawFlag(P(work, 0x294), FALSE);
    ManagedSprite_SetDrawFlag(P(work, 0x298), FALSE);
    BagApp_ShowPocketTabSprites(work, FALSE);
    BagApp_ShowCancelButton(work, 0);
    BagApp_DrawSelectedItemEntry(work, *(s16 *)(slot + 6), S32(work, 0x644) - 8);
    BagApp_ShowSelectedItemIcon(work, S32(work, 0x644) - 8);
    BagApp_ShowItemDescription(work, work->raw + 4, U16((BagWorkRaw *)P(work, 0x234), 0x66));
    BagApp_HideCursor(work);

    if (GetItemAttr(U16((BagWorkRaw *)P(work, 0x234), 0x66), 3, (enum HeapID)6) != 0 || S32(work, 0x684) == 0) {
        str = NewString_ReadMsgData(P(work, 0x2f0), 0x4c);
        BufferItemName(P(work, 0x2f4), zero, U16((BagWorkRaw *)P(work, 0x234), 0x66));
        StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
        String_Delete(str);
        U8(work, 0x616) = BagApp_PrintMessage(work, FALSE);
        return 0x18;
    }

    BagApp_PrintMoney(work, FALSE);
    BagApp_PrintSellPrice(work);
    BagApp_PrintQuantityDigits(work, 2);
    if (Pocket_GetQuantity(*(ItemSlot **)((u8 *)P(work, 0x234) + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12 + 4), *(u8 *)((u8 *)P(work, 0x234) + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12 + 0xd), U16((BagWorkRaw *)P(work, 0x234), 0x66), 6) == 1) {
        str = NewString_ReadMsgData(P(work, 0x2f0), 0x4e);
        BufferIntegerAsString(P(work, 0x2f4), 0, S16(work, 0x680) * S32(work, 0x684), 6, (PrintingMode)0, 1);
        StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
        String_Delete(str);
        U8(work, 0x616) = BagApp_PrintMessage(work, TRUE);
        return 0x15;
    }
    U16(work, 0x682) = *(u16 *)((u8 *)*(void **)slot + (*(s16 *)(slot + 6) + S32(work, 0x644) - 8) * 4 + 2);
    str = NewString_ReadMsgData(P(work, 0x2f0), 0x4d);
    BufferItemName(P(work, 0x2f4), zero, U16((BagWorkRaw *)P(work, 0x234), 0x66));
    StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
    String_Delete(str);
    U8(work, 0x616) = BagApp_PrintMessage(work, TRUE);
    return 0x11;
}

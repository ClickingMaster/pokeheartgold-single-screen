#include "overlay15_bag_quantity_branch_private.h"

int ov15_021FCB64(BagWorkRaw *work) {
    u8 *slot = (u8 *)P(work, 0x234) + 4 + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12;
    String *str;
    s32 attr;
    u32 zero;

    ov15_021FD574(work, 4, 0, 0);
    U16(work, 0x680) = 1;
    attr = GetItemAttr(U16((BagWorkRaw *)P(work, 0x234), 0x66), 0, (enum HeapID)6);
    zero = 0;
    S32(work, 0x684) = attr >> 1;
    ov15_021FD43C(P(work, 0), 5, 0);
    ScheduleBgTilemapBufferTransfer(P(work, 0), 5);
    ManagedSprite_SetDrawFlag(P(work, 0x294), FALSE);
    ManagedSprite_SetDrawFlag(P(work, 0x298), FALSE);
    ov15_02200458(work, FALSE);
    ov15_021FD788(work, 0);
    ov15_021FF4EC(work, *(s16 *)(slot + 6), S32(work, 0x644) - 8);
    ov15_022002B4(work, S32(work, 0x644) - 8);
    ov15_021FECA0(work, work->raw + 4, U16((BagWorkRaw *)P(work, 0x234), 0x66));
    ov15_021FFF24(work);

    if (GetItemAttr(U16((BagWorkRaw *)P(work, 0x234), 0x66), 3, (enum HeapID)6) != 0 || S32(work, 0x684) == 0) {
        str = NewString_ReadMsgData(P(work, 0x2f0), 0x4c);
        BufferItemName(P(work, 0x2f4), zero, U16((BagWorkRaw *)P(work, 0x234), 0x66));
        StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
        String_Delete(str);
        U8(work, 0x616) = ov15_021FEF48(work, FALSE);
        return 0x18;
    }

    ov15_021FF0FC(work, FALSE);
    ov15_021FF068(work);
    ov15_021FEDEC(work, 2);
    if (Pocket_GetQuantity(*(ItemSlot **)((u8 *)P(work, 0x234) + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12 + 4), *(u8 *)((u8 *)P(work, 0x234) + U8((BagWorkRaw *)P(work, 0x234), 0x64) * 12 + 0xd), U16((BagWorkRaw *)P(work, 0x234), 0x66), 6) == 1) {
        str = NewString_ReadMsgData(P(work, 0x2f0), 0x4e);
        BufferIntegerAsString(P(work, 0x2f4), 0, S16(work, 0x680) * S32(work, 0x684), 6, (PrintingMode)0, 1);
        StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
        String_Delete(str);
        U8(work, 0x616) = ov15_021FEF48(work, TRUE);
        return 0x15;
    }
    U16(work, 0x682) = *(u16 *)((u8 *)*(void **)slot + (*(s16 *)(slot + 6) + S32(work, 0x644) - 8) * 4 + 2);
    str = NewString_ReadMsgData(P(work, 0x2f0), 0x4d);
    BufferItemName(P(work, 0x2f4), zero, U16((BagWorkRaw *)P(work, 0x234), 0x66));
    StringExpandPlaceholders(P(work, 0x2f4), P(work, 0x5e4), str);
    String_Delete(str);
    U8(work, 0x616) = ov15_021FEF48(work, TRUE);
    return 0x11;
}

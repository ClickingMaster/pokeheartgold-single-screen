#include "to52_overlay15_bag_register_flow_private.h"

#define REFRESH_SELECTION(work, args, mode)                                                     \
    do {                                                                                        \
        ov15_02200294(work);                                                                    \
        ov15_021FF560(work);                                                                    \
        ov15_021FF7AC((u8 *)(work) + 0x184);                                                    \
        (args) = PTR(work, 0x234);                                                              \
        ov15_02200140(work, (u8 *)(args) + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), mode); \
        ov15_021FE868(work);                                                                    \
        ov15_021FED3C(work);                                                                    \
        ov15_021FB518(work);                                                                    \
    } while (0)

s32 ov15_021FC164(void *work) {
    BagArgs *args;
    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        ClearFrameAndWindow2((u8 *)work + 0x34, TRUE);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        Pocket_TakeItem(args->entries[args->selected].slots, args->entries[args->selected].quantity, args->item, U16(work, 0x680), 6);
        ov15_021F9F08(work);
        args = PTR(work, 0x234);
        ov15_02200140(work, (u8 *)args + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), TRUE);
        ov15_021FE868(work);
        ov15_021FED3C(work);
        ov15_021FB518(work);
        ov15_021FA170(work);
        ov15_021FD788(work, TRUE);
        return TRUE;
    }
    return 0xB;
}

s32 ov15_021FC224(void *work) {
    void *args = PTR(work, 0x234);
    void *str;
    if (Bag_TryRegisterItem(PTR(work, 0x238), U16(args, 0x66)) == 0) {
        str = NewString_ReadMsgData(PTR(work, 0x2F0), 0x67);
        FillWindowPixelBuffer((u8 *)work + 0x34, 0xF);
        StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), str);
        String_Delete(str);
        ov15_021FEF48(work, 0);
        ov15_02200294(work);
        ov15_021FFF24(work);
        ov15_021FD788(work, FALSE);
        return 0x19;
    }
    REFRESH_SELECTION(work, args, FALSE);
    return TRUE;
}

s32 ov15_021FC2E0(void *work) {
    void *args;
    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        ClearFrameAndWindow2((u8 *)work + 0x34, TRUE);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        REFRESH_SELECTION(work, args, FALSE);
        ov15_021FD788(work, TRUE);
        return TRUE;
    }
    return 0x19;
}

s32 ov15_021FC37C(void *work) {
    void *args;
    Bag_UnregisterItem(PTR(work, 0x238), U16(PTR(work, 0x234), 0x66));
    REFRESH_SELECTION(work, args, FALSE);
    return TRUE;
}

#include "to50_overlay15_bag_toss_confirm_private.h"
s32 ov15_021FC01C(void *work) {
    void *args;
    void *str;
    u32 input = YesNoPrompt_HandleInput(PTR(work, 0x804));
    switch (input) {
    case 1:
        ov15_021FF058(work);
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
        U8(work, 0x616) = ov15_021FEF48(work, 0);
        return 10;
    case 2:
        ov15_021FF058(work);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        ov15_02200140(work, (u8 *)args + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), 0);
        ov15_021FE868(work);
        ov15_021FED3C(work);
        ov15_021FD788(work, 1);
        ov15_021FB518(work);
        return 1;
    case (u32)-1:
        break;
    }
    return 9;
}

#include "to50_overlay15_bag_sale_flow_private.h"

s32 ov15_021FD10C(void *work) {
    void *args;
    void *string;
    u32 input = YesNoPrompt_HandleInput(PTR(work, 0x804));

    switch (input) {
    case 1:
        ov15_021FF058(work);
        string = NewString_ReadMsgData(PTR(work, 0x2F0), 0x4F);
        if (S16(work, 0x680) > 1) {
            BufferItemNamePlural(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        } else {
            BufferItemName(PTR(work, 0x2F4), 0, U16(PTR(work, 0x234), 0x66));
        }
        BufferIntegerAsString(PTR(work, 0x2F4), 1, S16(work, 0x680) * U32(work, 0x684), 6, 0, 1);
        StringExpandPlaceholders(PTR(work, 0x2F4), PTR(work, 0x5E4), string);
        String_Delete(string);
        U8(work, 0x616) = ov15_021FEF48(work, 0);
        return 0x17;
    case 2:
        ov15_021FF058(work);
        U32(work, 0x684) = 0;
        EraseFrameAndWindow1((u8 *)work + 0x214, 1);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        ov15_02200140(work, (u8 *)args + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), FALSE);
        ov15_021FE868(work);
        ov15_021FED3C(work);
        ov15_021FB518(work);
        ov15_02200458(work, 1);
        ov15_021FD788(work, 1);
        return 0x10;
    case (u32)-1:
        break;
    }
    return 0x16;
}

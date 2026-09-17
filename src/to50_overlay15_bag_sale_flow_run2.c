#include "to50_overlay15_bag_sale_flow_private.h"
s32 ov15_021FD2FC(void *work) {
    void *args;

    if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
        U32(work, 0x684) = 0;
        sub_0200E5D4((u8 *)work + 0x214, 1);
        ClearFrameAndWindow2((u8 *)work + 0x34, 1);
        ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
        ScheduleWindowCopyToVram((u8 *)work + 4);
        args = PTR(work, 0x234);
        ov15_02200140(work, (u8 *)args + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), TRUE);
        ov15_021FE868(work);
        ov15_021FED3C(work);
        ov15_021FB518(work);
        ov15_021FA170(work);
        ov15_02200458(work, 1);
        ov15_021FD788(work, 1);
        return 0x10;
    }
    return 0x18;
}

#include "to50_overlay15_bag_discard_state_private.h"
s32 ov15_021FBBB0(void *work) {
    void *args;
    switch (U8(work, 0x67B)) {
    case 0:
        U8(work, 0x616) = ov15_021FEF48(work, 0);
        U8(work, 0x67B) = 1;
        break;
    case 1:
        if (!TextPrinterCheckActive(U8(work, 0x616)) && ((U32(gSystem, 0x48) & 3) || U16(gSystem, 0x64))) {
            ov15_021FED3C(work);
            ClearFrameAndWindow2((u8 *)work + 0x34, 1);
            ClearWindowTilemapAndScheduleTransfer((u8 *)work + 0x34);
            ov15_021FBB28(work);
            ov15_021FB518(work);
            args = PTR(work, 0x234);
            ov15_02200140(work, (u8 *)args + 4 + U8(args, 0x64) * 12, ov15_021FA074(work), 1);
            ov15_021FA170(work);
            ScheduleWindowCopyToVram((u8 *)work + 4);
            ov15_021FD788(work, 1);
            U8(work, 0x67B) = 0;
            return 1;
        }
        break;
    }
    return 0xD;
}

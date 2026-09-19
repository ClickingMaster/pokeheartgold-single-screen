#include "to50_overlay80_menu_build_e948_private.h"

void ov80_0222E948(void *work) {
    u32 width;
    void *map;

    map = FrontierSystem_GetFrontierMap(PTR(work, 0));
    width = ov80_0222EB14(work);
    if ((width & 7) == 0) {
        width >>= 3;
    } else {
        width = (width >> 3) + 1;
    }
    if (((u32)U8(work, 0x97) << 25) >> 31) {
        U8(work, 0x98) -= width;
    }
    if (U8(work, 0x9B) > 8) {
        if (((u32)U8(work, 0x97) << 24) >> 31) {
            U8(work, 0x99) -= 16;
        }
        AddWindowParameterized(PTR(map, 0), (u8 *)work + 8, 1, U8(work, 0x98), U8(work, 0x99), (u8)width, 16, 14, 1);
    } else {
        if (((u32)U8(work, 0x97) << 24) >> 31) {
            U8(work, 0x99) -= U8(work, 0x9B) * 2;
        }
        AddWindowParameterized(PTR(map, 0), (u8 *)work + 8, 1, U8(work, 0x98), U8(work, 0x99), (u8)width, U8(work, 0x9B) * 2, 14, 1);
    }
    DrawFrameAndWindow1((u8 *)work + 8, TRUE, 0x3D9, 12);
    ov80_0222EB54(work);
    PTR(work, 0x1B4) = ListMenuInit((u8 *)work + 0x194, 0, U8(work, 0x96), (u8)U32(PTR(work, 0), 0x34));
    ov80_0222EE7C(work);
    PTR(work, 4) = SysTask_CreateOnMainQueue(ov80_0222EC90, work, 0);
}

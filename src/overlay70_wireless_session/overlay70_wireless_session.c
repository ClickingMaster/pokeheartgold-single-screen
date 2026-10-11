#include "private.h"

int ov70_02244854(void *work) {
    WirelessStatusIcon_Destroy();
    ov70_02244FA4(work, PTR_AT(work, 0xBAC), 0x1B, 1, 0xF0F);
    ov70_02238D84(work, 0x14, 0xB);
    return 3;
}

void ov70_02244888(void *work) {
    sub_02039418(PTR_AT(PTR_AT(work, 0), 0x20));
    Sys_SetSleepDisableFlag(4);
    ov70_02245124(work);
    ov00_021EC3F0((u8 *)work + 0x54, 2, 1, 0x14);
    ov00_021EC454(2);
    ov00_021EC4A4();
    U32_AT(work, 0x2C) = 3;
}

int ov70_022448C0(void *work) {
    u8 temp[0x48];

    ov00_021EC60C();
    if (ov00_021EC5B4() != 0) {
        switch (ov00_021EC724()) {
        case 7:
            U32_AT(work, 0x40) = ov00_021EC11C((u32 *)(temp + 8), (u32 *)(temp + 4));
            U32_AT(work, 0x44) = U32_AT(temp, 8);
            U32_AT(work, 0x48) = U32_AT(temp, 4);
            ov00_021EC210();
            ov00_021EC8D8();
            ov70_02238F80(work);
            U32_AT(work, 0x2C) = 0x17;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        default:
            ov00_021EC0FC(temp);
            ov70_02238F80(work);
            U32_AT(work, 0x2C) = 0x15;
            U32_AT(work, 0x3C) = -2;
            break;
        case 4:
            ov00_021EC9E0(temp + 0xC);
            U32_AT(work, 0x2C) = 4;
            break;
        }
    }
    return 3;
}

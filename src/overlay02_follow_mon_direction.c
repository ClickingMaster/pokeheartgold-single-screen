#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224F64C(FieldSystem *fieldSystem, void *arg1) {
    switch (Field_GetTimeOfDay(fieldSystem)) {
    case 0:
        *(u8 *)((u8 *)arg1 + 0x14) = 1;
        break;
    case 1:
        *(u8 *)((u8 *)arg1 + 0x14) = 2;
        break;
    case 2:
        *(u8 *)((u8 *)arg1 + 0x14) = 3;
        break;
    case 3:
        *(u8 *)((u8 *)arg1 + 0x14) = 4;
        break;
    case 4:
        *(u8 *)((u8 *)arg1 + 0x14) = 5;
        break;
    default:
        GF_AssertFail();
        *(u8 *)((u8 *)arg1 + 0x14) = 0;
        break;
    }
}

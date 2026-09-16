#include "overlay02_port_private.h"

WIP_LOCAL void ov02_0224A700(SysTask *task, void *data) {
    int winin;
    int winout;

    if (*(int *)((u8 *)data + 0x2c) == 0) {
        return;
    }
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & 0xFFFF1FFF) | (*(int *)((u8 *)data + 0x30) << 13);
    winin = (reg_G2_WININ & ~0x3f) | *(int *)((u8 *)data + 0x34);
    if (*(int *)((u8 *)data + 0x38) != 0) {
        winin |= 0x20;
    }
    reg_G2_WININ = winin;
    winout = (reg_G2_WINOUT & ~0x3f) | *(int *)((u8 *)data + 0x3c);
    if (*(int *)((u8 *)data + 0x40) != 0) {
        winout |= 0x20;
    }
    reg_G2_WINOUT = winout;
    {
        int u0 = *(int *)((u8 *)data + 0x50) / 0x1000;
        int v0 = *(int *)((u8 *)data + 0x4c) / 0x1000;
        reg_G2_WIN0H = (*(int *)((u8 *)data + 0x44) / 0x1000) << 8 & 0xff00 | (u8)(*(int *)((u8 *)data + 0x48) / 0x1000);
        reg_G2_WIN0V = v0 << 8 & 0xff00 | (u8)u0;
    }
}

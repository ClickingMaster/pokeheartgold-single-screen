#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224F880(void *a0, int a1) {
    int i;
    u8 *p;
    ReadWholeNarcMemberByIdPair((u8 *)a0 + 0x7e4, (NarcId)0xdf, a1 - 1);
    *(u8 *)((u8 *)a0 + 0x868) = 0;
    *(u8 *)((u8 *)a0 + 0x869) = 0;
    *(u8 *)((u8 *)a0 + 0x86b) = 0;
    ((ov02_FollowMonStep *)((u8 *)a0 + 0x86c))->idx = 0;
    *(u8 *)((u8 *)a0 + 0x86a) = 0;
    *(u8 *)((u8 *)a0 + 0x86d) = 0;
    *(u16 *)((u8 *)a0 + 0x86e) = 0;
    for (i = 0, p = (u8 *)a0; i < 5; i++, p += 8) {
        if (*(u16 *)(p + 0x7e4) == 0xFFFF) {
            break;
        }
    }
    ((ov02_FollowMonStep *)((u8 *)a0 + 0x86c))->hi = i;
}

#include "global.h"
extern u8 _0224CF98[];
extern void ov13_02222978(void *, int, u32);
extern int RTC_GetTime(void *);
u16 ov13_02222AB0(void);
u16 ov13_02222AB0(void) {
    u32 time[3];
    if (((u32 *)_0224CF98)[2] == 0) {
        u32 seed = 0;
        ov13_02222978(time, 0, 12);
        if (RTC_GetTime(time) == 0) {
            seed += time[0] << 10;
            seed += time[1] << 3;
            seed += time[2];
        }
        ((u32 *)_0224CF98)[6] = seed;
        ((u32 *)_0224CF98)[7] = 0x5D588B65;
        ((u32 *)_0224CF98)[8] = 0x269EC3;
        ((u32 *)_0224CF98)[2] = 1;
    }
    ((u32 *)_0224CF98)[6] = ((u32 *)_0224CF98)[7] * ((u32 *)_0224CF98)[6] + ((u32 *)_0224CF98)[8];
    return (u16)(((((u32 *)_0224CF98)[6] >> 16) * 0x7FFF) >> 16);
}

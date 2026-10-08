#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {
    u8 pad0[0x4b2];
    u8 f4b2;
    u8 f4b3;
    u8 pad1;
    u8 f4b5;
    u8 pad2[2];
    u32 f4b8;
} BeaconGameInfo;
typedef struct {
    u8 pad0[0x1c];
    BeaconGameInfo *gameInfo; /* 0x1c */
    u8 *src;                  /* 0x20 */
    u8 state;                 /* 0x24 */
    u8 pad25;
    u8 f26;
    u8 seqNo;                 /* 0x27 */
    u8 seqMax;                /* 0x28 */
    u8 counter;               /* 0x29 */
    u8 pad2a[0x16];
    u32 f40;
    u8 lo : 2;
    u8 hi : 6;                /* 0x44 */
    u8 f45;
    u8 f46;
    u8 f47;
    u16 sum;                  /* 0x48 */
    u8 f4a;
    u8 f4b;
    u8 len;                   /* 0x4c */
} BeaconState;
extern BeaconState ov13_022474A0;
extern u8 bsendBuff[];
extern u16 MBi_calc_cksum(void *buf, u32 len);
extern int WM_SetGameInfo(void *callback, void *buf, u16 size, u16 a, u32 b, u8 c);
void MBi_SendFixedBeacon(u16 a, u32 b, u32 c);
#define S ov13_022474A0
#pragma push
#pragma optimization_level 2
#pragma opt_common_subs off
// @fn MBi_SendFixedBeacon
void MBi_SendFixedBeacon(u16 a, u32 b, u32 c) {
    u32 end = (u32)S.gameInfo + 0x358;
    u32 len = (u32)S.src + 0x62;
    if (len <= end) {
        S.len = 0x62;
    } else {
        len = end - (u32)S.src;
        S.len = len;
        {
            u8 n = S.len;
            MIi_CpuClear16(0, (u16 *)(bsendBuff + 0xE + n), 0x62 - n);
        }
    }
    MIi_CpuCopy16((u16 *)S.src, (u16 *)(bsendBuff + 0xE), S.len);
    S.f4a = S.seqNo;
    S.f4b = S.seqMax;
    S.lo = S.gameInfo->f4b2;
    S.f45 = S.gameInfo->f4b3;
    S.f46 = S.f26;
    S.f40 = S.gameInfo->f4b8;
    S.hi = S.gameInfo->f4b5;
    S.f47 = S.counter++;
    S.sum = 0;
    S.sum = MBi_calc_cksum(bsendBuff + 8, 0x68);
    if (++S.seqNo < S.seqMax) {
        S.src += 0x62;
    } else {
        S.state = 4;
    }
    WM_SetGameInfo(NULL, bsendBuff, 0x70, a, b, c | 3);
}
#pragma pop

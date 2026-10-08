#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u8 data[0x16]; } BeaconMember;
typedef struct {
    u8 pad0[0x358];
    u8 f358;
    u8 pad359;
    u16 f35a;
    u16 f35c;
    BeaconMember members[15]; /* 0x35e */
    u8 pad[0x4a8 - 0x35e - 15 * 0x16];
    u8 f4a8[8];
    u16 f4b0;
    u8 pad4b2;
    u8 f4b3;
    u8 f4b4;
    u8 f4b5;
    u8 pad4b6[2];
    u32 f4b8;
} BeaconGameInfo;
typedef struct {
    u8 pad0[0x10];
    u32 mode;                 /* 0x10 */
    void (*callback)(u32);    /* 0x14 */
    u8 pad18[4];
    BeaconGameInfo *gameInfo; /* 0x1c */
    u8 *src;                  /* 0x20 */
    u8 state;                 /* 0x24 */
    u8 pad25;
    u8 f26;
    u8 seqNo;
    u8 seqMax;
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
    u8 pad4b;
    u16 f4c;
    u16 f4e;
} BeaconState;
extern BeaconState ov13_022474A0;
extern u8 bsendBuff[];
extern u16 MBi_calc_cksum(void *buf, u32 len);
extern void MBi_InitSendVolatBeacon(void);
extern int WM_SetGameInfo(void *callback, void *buf, u16 size, u16 a, u32 b, u8 c);
void MBi_SendVolatBeacon(u16 a, u32 b, u32 c);
#define S ov13_022474A0
#pragma push
#pragma optimization_level 2
#pragma opt_common_subs off
// @fn MBi_SendVolatBeacon
void MBi_SendVolatBeacon(u16 a, u32 b, u32 c) {
    int i;
    u32 n;
    u16 diff;
    if (S.f26 != S.gameInfo->f4b4) {
        MBi_InitSendVolatBeacon();
    }
    S.lo = 2;
    S.f45 = S.gameInfo->f4b3;
    S.f46 = S.f26;
    S.f40 = S.gameInfo->f4b8;
    S.hi = S.gameInfo->f4b5;
    S.f47 = S.counter++;
    S.f4a = S.gameInfo->f358;
    S.f4c = S.gameInfo->f35a;
    S.f4e = S.gameInfo->f35c;
    if (S.mode == 0 && S.callback != NULL) {
        S.callback(S.gameInfo->f4b8);
    }
    for (i = 0; i < 8; i++) {
        (bsendBuff + 0x68)[i] = S.gameInfo->f4a8[i];
    }
    MIi_CpuClear16(0, (u16 *)(bsendBuff + 0x10), 0x58);
    diff = S.gameInfo->f4b0 ^ S.gameInfo->f35a;
    n = 0;
    for (i = 0; i < 15; i++) {
        if ((2 << i) & diff) {
            MIi_CpuCopy16((u16 *)&S.gameInfo->members[i], (u16 *)(bsendBuff + 0x10 + n * 0x16), 0x16);
            n++;
            S.gameInfo->f4b0 |= 2 << i;
            if (n == 4) {
                break;
            }
        }
    }
    if (n < 4) {
        bsendBuff[0x10 + n * 0x16] &= ~0xf0;
    }
    S.sum = 0;
    S.sum = MBi_calc_cksum(bsendBuff + 8, 0x68);
    if (S.gameInfo->f4b0 == S.gameInfo->f35a) {
        S.state = 1;
    }
    WM_SetGameInfo(NULL, bsendBuff, 0x70, a, b, c | 3);
    if (S.mode == 1 && S.callback != NULL) {
        S.callback(S.gameInfo->f4b8);
    }
}
#pragma pop

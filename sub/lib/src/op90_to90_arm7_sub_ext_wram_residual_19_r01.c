#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {
    u8 pad0[0x1c];
    u32 mpScheduled;   /* 0x1c */
    u8 pad20[0x42 - 0x20];
    s16 mpVCount;      /* 0x42 */
    u8 pad44[0xd0 - 0x44];
    u32 vsyncTime;     /* 0xd0 */
    u32 lastVCount;    /* 0xd4 */
    u32 adjust;        /* 0xd8 */
} WmspStatus;
typedef struct {
    u8 pad[0x1550];
    WmspStatus *status;
} WmspWork;
extern WmspWork wmspW;
extern u8 wmspVAlarm[];
extern void __VENEER_OS_SetVAlarm(void *alarm, s16 count, s16 delay, void (*handler)(void *), void *arg);
extern void WmspChildAdjustVSync2(void *);
extern void WmspChildVAlarmMP(void *);
#define REG_TSF0 (*(volatile u16 *)0x048080f8)
#define REG_TSF1 (*(volatile u16 *)0x048080fa)
#define REG_VCOUNT (*(volatile u16 *)0x04000006)
void WmspChildAdjustVSync1(void *arg);

static inline void WmspChildCalcVSyncAdjust(void) {
    WmspStatus *status = wmspW.status;
    u16 tsfLo, tsfLo2;
    u32 tsfHi;
    u32 now;
    int i;

    status->vsyncTime <<= 6;
    tsfLo = REG_TSF0;
    tsfHi = REG_TSF1;
    tsfLo2 = REG_TSF0;
    if (tsfLo > tsfLo2) {
        tsfHi = REG_TSF1;
    }
    tsfHi = (tsfLo2 | (tsfHi << 16)) & 0x3fffc0;
    now = ((263 - REG_VCOUNT) * 127 + (tsfHi << 1)) >> 1 & 0x3fffc0;
    if (status->vsyncTime > now) {
        status->adjust = 0;
        return;
    }
    for (i = 1; i < 30; i++) {
        status->vsyncTime += 0x414b;
        if (status->vsyncTime > now) {
            status->adjust = status->vsyncTime - now;
            if (status->adjust > 0x400e) {
                status->adjust = 0;
            }
            return;
        }
    }
    status->adjust = 0;
}

// @fn WmspChildAdjustVSync1
void WmspChildAdjustVSync1(void *arg) {
    WmspStatus *status = wmspW.status;
    u32 vcount = *(volatile u16 *)0x0380fff0;
    status->vsyncTime = vcount;
    if (status->lastVCount != vcount) {
        status->lastVCount = vcount;
        WmspChildCalcVSyncAdjust();
    }
    if (status->adjust > 127) {
        __VENEER_OS_SetVAlarm(wmspVAlarm, 208, 263, WmspChildAdjustVSync2, (void *)2);
    } else {
        status->mpScheduled = 1;
        __VENEER_OS_SetVAlarm(wmspVAlarm, status->mpVCount, 263, WmspChildVAlarmMP, (void *)4);
    }
}

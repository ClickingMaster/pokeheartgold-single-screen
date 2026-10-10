#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 lo : 4, hi : 4;
    u8 data[21];
} Entry;

typedef struct {
    u8 pad[0x358];
    u8 field, pad359;
    u16 mask, other;
    Entry entries[15];
    u8 data[8];
    u16 sentMask;
    u8 pad4b2, id, gen, high;
    u16 pad4b6;
    u32 version;
} Parent;

typedef struct {
    u8 pad[0x10];
    int mode;
    void (*callback)(u32);
    u32 pad18;
    Parent *parent;
    u8 pad20[4];
    u8 sent, pad25, gen, pad27[2], seq, pad2a[0x16];
    u32 version;
    u8 type : 2, high : 6;
    u8 id, generation, sequence;
    u16 checksum;
    u8 field, pad4b;
    u16 mask, other;
} Work;

typedef struct {
    u8 data[4];
    u16 channel;
} Scan;

typedef struct {
    u8 pad[8];
    u8 *work;
} Global;

extern Work ov13_022474A0;
extern u8 bsendBuff[];
extern Global ov13_0224CF40;
extern u16 ov13_0224598C[];
extern void MBi_InitSendVolatBeacon(void);
extern u16 MBi_calc_cksum(const void *, int);
extern int WM_SetGameInfo(void *, const void *, int, u32, u32, int);
extern u16 WM_GetAllowedChannel(void);
extern void MBi_ParentCallback(void *);
extern int WM_SetIndCallback(void (*)(void *));
extern int WM_SetLifeTime(void (*)(void *), int, int, int, int);
extern void MBi_CheckWmErrcode(int, int);
void MBi_SendVolatBeacon(u32, u32, int);
int changeScanChannel(Scan *);
int MBi_IsSendEnabled(void);
void MBi_OnInitializeDone(void);

static inline int isParentReady(void) {
    u8 *p = ov13_0224CF40.work;
    return *(u16 *)(p + 0x528) == 1 && p[0x50c] == 0;
}

static inline int isParentFree(void) {
    u8 *p = ov13_0224CF40.work;
    return isParentReady() && *(u16 *)(p + 0x526) == 0;
}

int changeScanChannel(Scan *scan) {
    int allowed = WM_GetAllowedChannel();
    u16 channel, i;
    if (allowed == 0) {
        return 0;
    }
    channel = scan->channel;
    i = 0;
    do {
        if ((allowed & (1 << (channel - 1))) && scan->channel != channel) {
            scan->channel = channel;
            break;
        }
        i++;
        channel = channel == 16 ? 1 : channel + 1;
    } while (i < 16);
    return 1;
}

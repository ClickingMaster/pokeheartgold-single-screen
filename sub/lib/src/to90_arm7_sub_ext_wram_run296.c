#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35628Flags {
    u16 beacon : 1, resBc : 1, preamble : 1, antenna : 1, diversity : 1, antennaOther : 1, bcnInd : 1, nullKey : 1, other : 8;
} W35628Flags;

typedef struct W35628Config {
    u8 pad[0x22];
    u16 maxConn;
    u16 mac[3];
    u16 retry, enable, mode, rate, auth, wep, key;
    u16 pad38;
    W35628Flags flags;
    u16 active;
    u8 pad3e[0xe];
    u16 revision;
    u8 pad4e[0x36];
    u16 ssidMask[16];
    u8 pada4[0x1a];
    u16 enable2;
    u8 padc0[2];
    u16 lost;
} W35628Config;

typedef struct W35628Work {
    u8 pad[0x300];
    W35628Config config;
} W35628Work;

#define W35628_WORK (*(W35628Work *volatile *)0x0380fff4)
extern int WSetListenInterval(u16);
int PARAMSET_ListenIntervalReqCmd(u16 *, u16 *);

int PARAMSET_ListenIntervalReqCmd(u16 *req, u16 *out) {
    int mode;
    out[1] = 1;
    mode = W35628_WORK->config.mode;
    if (mode != 2 && mode != 3) {
        return 11;
    }
    return WSetListenInterval(req[8]);
}

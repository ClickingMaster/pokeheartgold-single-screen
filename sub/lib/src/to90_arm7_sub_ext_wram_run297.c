#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35629Flags {
    u16 beacon : 1, resBc : 1, preamble : 1, antenna : 1, diversity : 1, antennaOther : 1, bcnInd : 1, nullKey : 1, other : 8;
} W35629Flags;

typedef struct W35629Config {
    u8 pad[0x22];
    u16 maxConn;
    u16 mac[3];
    u16 retry, enable, mode, rate, auth, wep, key;
    u16 pad38;
    W35629Flags flags;
    u16 active;
    u8 pad3e[0xe];
    u16 revision;
    u8 pad4e[0x36];
    u16 ssidMask[16];
    u8 pada4[0x1a];
    u16 enable2;
    u8 padc0[2];
    u16 lost;
} W35629Config;

typedef struct W35629Work {
    u8 pad[0x300];
    W35629Config config;
} W35629Work;

#define W35629_WORK (*(W35629Work *volatile *)0x0380fff4)
extern int WSetGameInfo(u32, void *);
int PARAMSET_GameInfoReqCmd(u16 *, u16 *);

int PARAMSET_GameInfoReqCmd(u16 *req, u16 *out) {
    int length;
    out[1] = 1;
    if (W35629_WORK->config.mode != 1) {
        return 11;
    }
    length = req[8];
    if (req[7] < ((length + 1) / 2 + 1)) {
        return 4;
    }
    return WSetGameInfo(length, req + 9);
}

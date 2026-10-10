#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35634Flags {
    u16 beacon : 1, resBc : 1, preamble : 1, antenna : 1, diversity : 1, antennaOther : 1, bcnInd : 1, nullKey : 1, other : 8;
} W35634Flags;

typedef struct W35634Config {
    u8 pad[0x22];
    u16 maxConn;
    u16 mac[3];
    u16 retry, enable, mode, rate, auth, wep, key;
    u16 pad38;
    W35634Flags flags;
    u16 active;
    u8 pad3e[0xe];
    u16 revision;
    u8 pad4e[0x36];
    u16 ssidMask[16];
    u8 pada4[0x1a];
    u16 enable2;
    u8 padc0[2];
    u16 lost;
} W35634Config;

typedef struct W35634Work {
    u8 pad[0x300];
    W35634Config config;
} W35634Work;

#define W35634_WORK (*(W35634Work *volatile *)0x0380fff4)
int PARAMGET_ModeReqCmd(u16 *, u16 *);

int PARAMGET_ModeReqCmd(u16 *req, u16 *out) {
    out[1] = 2;
    out[3] = W35634_WORK->config.mode;
    return 0;
}

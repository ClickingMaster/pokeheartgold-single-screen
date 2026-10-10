#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35649Flags {
    u16 beacon : 1, resBc : 1, preamble : 1, antenna : 1, diversity : 1, antennaOther : 1, bcnInd : 1, nullKey : 1, other : 8;
} W35649Flags;

typedef struct W35649Config {
    u8 pad[0x22];
    u16 maxConn;
    u16 mac[3];
    u16 retry, enable, mode, rate, auth, wep, key;
    u16 pad38;
    W35649Flags flags;
    u16 active;
    u8 pad3e[0xe];
    u16 revision;
    u8 pad4e[0x36];
    u16 ssidMask[16];
    u8 pada4[0x1a];
    u16 enable2;
    u8 padc0[2];
    u16 lost;
} W35649Config;

typedef struct W35649Work {
    u8 pad[0x300];
    W35649Config config;
} W35649Work;

#define W35649_WORK (*(W35649Work *volatile *)0x0380fff4)
int PARAMGET_BcnSendRecvIndReqCmd(u16 *, u16 *);

int PARAMGET_BcnSendRecvIndReqCmd(u16 *req, u16 *out) {
    u16 revision;
    W35649Work *work;
    out[1] = 2;
    work = W35649_WORK;
    revision = work->config.revision;
    if (revision < 16) {
        return 1;
    }
    out[3] = work->config.flags.bcnInd;
    return 0;
}

#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct W35630Flags {
    u16 beacon : 1, resBc : 1, preamble : 1, antenna : 1, diversity : 1, antennaOther : 1, bcnInd : 1, nullKey : 1, other : 8;
} W35630Flags;

typedef struct W35630Config {
    u8 pad[0x22];
    u16 maxConn;
    u16 mac[3];
    u16 retry, enable, mode, rate, auth, wep, key;
    u16 pad38;
    W35630Flags flags;
    u16 active;
    u8 pad3e[0xe];
    u16 revision;
    u8 pad4e[0x36];
    u16 ssidMask[16];
    u8 pada4[0x1a];
    u16 enable2;
    u8 padc0[2];
    u16 lost;
} W35630Config;

typedef struct W35630Work {
    u8 pad[0x300];
    W35630Config config;
} W35630Work;

#define W35630_WORK (*(W35630Work *volatile *)0x0380fff4)
extern void WSetMacAdrs1(void *, const void *);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
int PARAMGET_AllReqCmd(u16 *, u16 *);

int PARAMGET_AllReqCmd(u16 *req, u16 *out) {
    out[1] = 33;
    WSetMacAdrs1(out + 3, W35630_WORK->config.mac);
    out[6] = W35630_WORK->config.retry;
    out[7] = W35630_WORK->config.enable;
    out[8] = W35630_WORK->config.enable2;
    out[9] = W35630_WORK->config.mode;
    out[10] = W35630_WORK->config.rate;
    out[11] = W35630_WORK->config.wep;
    out[12] = W35630_WORK->config.key;
    out[13] = W35630_WORK->config.flags.beacon;
    out[14] = W35630_WORK->config.flags.resBc;
    out[15] = W35630_WORK->config.lost;
    out[16] = W35630_WORK->config.active;
    __VENEER_MIi_CpuCopy16(W35630_WORK->config.ssidMask, out + 17, 32);
    out[33] = W35630_WORK->config.flags.preamble;
    out[34] = W35630_WORK->config.auth;
    return 0;
}

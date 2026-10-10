#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

struct State {
    u8 p[0x20];
    u32 server;
    u8 q[0x14];
    u32 retry;
    u8 z[0x10];
    u32 lease;
};
extern struct State _021D4620;
extern u8 _021D4B76[];
extern void CPS_SocUse(void);
extern void CPS_SocDatagramMode(void);
extern void CPS_SocRelease(void);
extern void CPS_SocBind(int, int, u32);
extern int dhcp_send_request(int);
extern int dhcp_analyze_response(int, int);
extern u8 *dhcp_setcommon(u8 *, int, int);
extern u8 *pad_mem(int, int, u8 *, int);
extern int CPS_SocWrite(void *, int);
int dhcp_request_server(u32 *, int);
void dhcp_release_server(void);

static inline void releaseBody(u8 *base) {
    u8 *p = dhcp_setcommon(base, 7, 0);
    *p++ = 255;
    p = pad_mem(0, 300, p, p - base);
    CPS_SocWrite(base, p - base);
    CPS_SocRelease();
}

int dhcp_request_server(u32 *out, int mode) {
    int result, i;
    u32 retry;
    CPS_SocUse();
    CPS_SocDatagramMode();
    if (mode == 1) {
        CPS_SocBind(68, 67, _021D4620.server);
    } else {
        CPS_SocBind(68, 67, -1);
    }
    for (i = 0; i < 4; i++) {
        result = dhcp_analyze_response(dhcp_send_request(mode), i);
        if (result) {
            break;
        }
    }
    CPS_SocRelease();
    if (result == 2) {
        *out = _021D4620.lease >> 1;
        _021D4620.retry = (_021D4620.lease * 3) >> 3;
        return 1;
    }
    retry = _021D4620.retry >> 1;
    _021D4620.retry = retry;
    *out = retry;
    switch (mode) {
    case 1:
        if (retry < 60) {
            *out = 1;
            _021D4620.retry = _021D4620.lease >> 3;
        }
        break;
    case 2:
        if (retry < 60) {
            *out = 1;
        }
        break;
    }
    return 0;
}

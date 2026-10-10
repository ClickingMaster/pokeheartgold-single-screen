#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u16 zero[6];
    u16 cmd;
    u16 len;
} W35303Req;

extern void WMSP_WlRequest(void *);
extern void *WMSPi_WL_NoArg(void *, u16, u16);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
void *WMSP_WL_ParamSetBeaconLostThreshold(void *buf, u16 a);

void *WMSP_WL_ParamSetBeaconLostThreshold(void *buf, u16 a) {
    W35303Req *q = buf;
    u16 *param = (u16 *)((u8 *)buf + 16);
    W35303Req *reply;
    q->zero[0] = 0;
    q->zero[1] = 0;
    q->zero[2] = 0;
    q->zero[3] = 0;
    q->zero[4] = 0;
    q->zero[5] = 0;
    q->cmd = 523;
    q->len = 1;
    param[0] = a;
    reply = (W35303Req *)((u8 *)buf + q->len * 2);
    *((u16 *)((u8 *)reply + 16)) = q->cmd;
    *((u16 *)((u8 *)reply + 18)) = 1;
    WMSP_WlRequest(buf);
    return (u8 *)reply + 16;
}

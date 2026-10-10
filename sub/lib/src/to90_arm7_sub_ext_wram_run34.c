#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u16 zero[6];
    u16 cmd;
    u16 len;
} W35319Req;

extern void WMSP_WlRequest(void *);
extern void *WMSPi_WL_NoArg(void *, u16, u16);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
void *WMSP_WL_DevSetInitializeWirelessCounter(void *buf);

void *WMSP_WL_DevSetInitializeWirelessCounter(void *buf) {
    return WMSPi_WL_NoArg(buf, 773, 1);
}

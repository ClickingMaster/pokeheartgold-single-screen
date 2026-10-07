#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u16 zero[6];u16 cmd;u16 len;} W35321Req;
extern void WMSP_WlRequest(void *);
extern void *WMSPi_WL_NoArg(void *,u16,u16);
extern void __VENEER_MIi_CpuCopy16(const void *,void *,u32);
void *WMSP_WL_DevGetWirelessCounter(void *buf);
// @fn WMSP_WL_DevGetWirelessCounter
void *WMSP_WL_DevGetWirelessCounter(void *buf){return WMSPi_WL_NoArg(buf,775,92);}

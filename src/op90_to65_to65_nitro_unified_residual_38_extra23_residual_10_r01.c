// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u8 pad[0x10]; u32 dcfReady; } WMStatusView;
typedef struct { u32 pad; WMStatusView *status; } WMSysWork;
typedef void (*WMCb)(void *);
extern WMSysWork *WMi_GetSystemWork(void);
extern int WMi_CheckStateEx(int,...);
extern void WMi_SetCallbackTable(int,WMCb);
extern int WMi_SendCommand(u16,u16,...);
int WM_SetDCFData(WMCb,const u8*,const u16*,u16);
// @fn WM_SetDCFData
int WM_SetDCFData(WMCb callback,const u8 *destAdr,const u16 *sendData,u16 sendDataSize){
 int result;
 WMSysWork *sys;
 u32 wMac[2];
 sys=WMi_GetSystemWork();
 result=WMi_CheckStateEx(1,0xb);
 if(result!=0)return result;
 DC_InvalidateRange(&sys->status->dcfReady,4);
 if(sys->status->dcfReady==0)return 3;
 if(sendDataSize>0x5e4)return 6;
 DC_StoreRange((void*)sendData,sendDataSize);
 WMi_SetCallbackTable(0x12,callback);
 MI_CpuCopy8(destAdr,wMac,6);
 result=WMi_SendCommand(0x12,4,wMac[0],wMac[1],sendData,sendDataSize);
 if(result!=0)return result;
 return 2;
}

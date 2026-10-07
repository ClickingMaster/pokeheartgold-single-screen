#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad0[12];u32 started;u8 pad10[0x8c];u16 mode;u8 pad9e[0x28];u16 state;u8 padc8[0xc0];u16 flag;} Status;
typedef struct {void*a;Status*status;} System;
typedef struct {u16 command,pad;void*receive;u32 receiveLength;void*send;u32 sendLength;u8 zero[28],params[16];} Command;
extern System *WMi_GetSystemWork(void);
extern int WMi_CheckStateEx(int,...),WM_GetMPReceiveBufferSize(void),WM_GetMPSendBufferSize(void),WMi_SendCommandDirect(void*,int);
extern void WMi_SetCallbackTable(int,void*);
int WMi_StartMP(void*,void*,u16,void*,u16,const void*);
int WMi_StartMP(void*callback,void*receive,u16 receiveSize,void*send,u16 sendSize,const void*params){Command command;Status*status=WMi_GetSystemWork()->status;int result;
 result=WMi_CheckStateEx(2,7,8);if(result)return result;
 DC_InvalidateRange(&status->flag,2);DC_InvalidateRange(&status->state,2);if(status->flag&&status->state!=1)return 3;
 DC_InvalidateRange(&status->started,4);if(status->started==1)return 3;
 if(receiveSize&63)return 6;if(sendSize&31)return 6;
 DC_InvalidateRange(&status->mode,2);if(!status->mode){if(receiveSize<WM_GetMPReceiveBufferSize())return 6;if(sendSize<WM_GetMPSendBufferSize())return 6;}
 WMi_SetCallbackTable(14,callback);MIi_CpuClear32(0,(u32*)&command,64);
 command.command=14;command.receive=receive;command.receiveLength=(u32)receiveSize>>1;command.send=send;command.sendLength=sendSize;
 MIi_CpuClear32(0,(u32*)command.zero,28);MIi_CpuCopy32((const u32*)params,(u32*)command.params,16);
 result=WMi_SendCommandDirect(&command,64);if(result==0)result=2;return result;}

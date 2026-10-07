#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Callback)(void*);
typedef struct{u8 pad[0xcc];Callback callbacks[16];void*args[16];u32 unused;u16 connected;}System;
typedef struct{u16 api,error,state,port;void*data;u32 length;u16 x10,x12;u8 mac[6];u16 x1a;void*arg;u16 aid,connected;u8 rest[0x44-0x24];}Event;
extern System*WMi_GetSystemWork(void);extern int WMi_CheckInitialized(void);extern u16 WM_GetConnectedAIDs(void),WM_GetAID(void);
int WM_SetPortCallback(u16,Callback,void*);
int WM_SetPortCallback(u16 port,Callback callback,void*arg){Event event;OSIntrMode irq;int error;System*work;if(callback){MI_CpuFill8(&event,0,sizeof(event));event.api=130;event.error=0;event.state=25;event.port=port;event.data=0;event.length=0;event.x10=0;event.x1a=65535;event.arg=arg;event.x12=0;OS_GetMacAddress(event.mac);}irq=OS_DisableInterrupts();error=WMi_CheckInitialized();if(error){OS_RestoreInterrupts(irq);return error;}work=WMi_GetSystemWork();work->callbacks[port]=callback;work->args[port]=arg;if(callback){event.connected=WM_GetConnectedAIDs();event.aid=WM_GetAID();callback(&event);}OS_RestoreInterrupts(irq);return 0;}

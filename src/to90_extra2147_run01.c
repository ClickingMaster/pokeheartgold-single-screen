#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Callback)(void*);typedef struct{u8 p[0x14c];u32 x14c;u16 x150;}System;
typedef struct{u16 id,pad;const u16*desc;u8 data[24];u32 arg;u16 pad24,arg26;}Request;
extern int WMi_CheckStateEx(int,...),WMi_SendCommandDirect(void*,u32);extern System*WMi_GetSystemWork(void);extern void WMi_SetCallbackTable(int,Callback);
int WM_StartConnectEx(Callback,const u16*,const void*,u32,u16);
int WM_StartConnectEx(Callback callback,const u16*desc,const void*data,u32 arg,u16 extra){Request request;int error=WMi_CheckStateEx(1,2);System*work;if(error)return error;if(!desc)return 6;DC_StoreRange(desc,*desc*2);work=WMi_GetSystemWork();work->x150=0;work->x14c=0;WMi_SetCallbackTable(12,callback);request.id=12;request.desc=desc;if(data)MI_CpuCopy8(data,request.data,24);else MI_CpuFill8(request.data,0,24);request.arg=arg;request.arg26=extra;error=WMi_SendCommandDirect(&request,sizeof(request));if(error==0)return 2;return error;}

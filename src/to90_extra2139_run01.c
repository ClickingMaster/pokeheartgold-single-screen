#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef void(*Callback)(void*);
extern u8 _021E4CC0[];extern int WMi_CheckStateEx(int,...),WMi_SendCommand(u16,u16,...);extern void WMi_SetCallbackTable(int,Callback);
int WM_SetGameInfo(Callback,const void*,u16,u32,u16,u8);
int WM_SetGameInfo(Callback cb,const void*src,u16 size,u32 id,u16 group,u8 flags){int error=WMi_CheckStateEx(2,7,9);if(error)return error;if(!src)return 6;if(size>112)return 6;MIi_CpuCopy16((const u16*)src,(u16*)_021E4CC0,size);DC_StoreRange(_021E4CC0,size);WMi_SetCallbackTable(24,cb);error=WMi_SendCommand(24,5,_021E4CC0,size,id,group,flags);if(!error)error=2;return error;}

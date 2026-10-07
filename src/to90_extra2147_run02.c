#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 word;u8 *status;} Sys;
extern Sys *WMi_GetSystemWork(void);
extern int WMi_CheckStateEx(int,...),WMi_SendCommand(int,...);
extern void WMi_SetCallbackTable(int,void (*)(void *));
int WM_Disconnect(void (*)(void *),u16);
int WM_Disconnect(void (*callback)(void *),u16 aid){Sys *sys=WMi_GetSystemWork();int result=WMi_CheckStateEx(5,7,9,8,10,11);
 if(result)return result;
 if(*(u16*)sys->status==7 || *(u16*)sys->status==9){
  if(aid<1 || aid>15)return 6;
  DC_InvalidateRange(sys->status+0x182,2);
  if(!(*(u16*)(sys->status+0x182)&(1<<aid)))return 7;
 }else if(aid!=0)return 6;
 WMi_SetCallbackTable(13,callback);result=WMi_SendCommand(13,1,1<<aid);
 if(result==0)result=2;return result;
}

#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern void *WMi_GetSystemWork(void);
extern int WMi_CheckStateEx(int,...);
extern int WMi_SendCommand(int,int,...);
int WM_SetMPDataToPortEx(void *,void *,const void *,u16,u16,u16,u16);
int WM_SetMPDataToPortEx(void *callback,void *arg,const void *data,u16 length,u16 dest,u16 port,u16 priority) {
 int ready=1; u8 *status=*(u8 **)((u8 *)WMi_GetSystemWork()+4); int result=WMi_CheckStateEx(2,9,10);
 if(result) return result;
 DC_InvalidateRange(status+0x188,2);
 if(!*(u16 *)(status+0x188)) {DC_InvalidateRange(status+0x182,2); ready=*(u16 *)(status+0x182); DC_InvalidateRange(status+0x86,2);}
 if(!data)return 6; if(!ready)return 7;
 DC_InvalidateRange(status+0x7c,2); if(data==*(void **)(status+0x7c))return 6;
 if(length>512)return 6; if(!length)return 6;
 DC_StoreRange(data,length);
 result=WMi_SendCommand(15,7,data,length,dest,port,priority,callback,arg); if(!result)return 2; return result;
}

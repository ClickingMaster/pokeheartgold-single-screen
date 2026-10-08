// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <stdarg.h>
#pragma force_active on
extern s32 PXI_SendWordByFifo(s32,u32,BOOL);
extern u32 *WmGetCommandBuffer4Arm7(void);
extern OSMessageQueue _021E4228;
int WMi_SendCommand(u16,u16,...);
// @fn WMi_SendCommand
int WMi_SendCommand(u16 id,u16 paramNum,...){
 va_list vlist;
 int i;
 u32 *buf;
 s32 result;
 buf=WmGetCommandBuffer4Arm7();
 if(buf==NULL)return 8;
 *(u16*)buf=id;
 va_start(vlist,paramNum);
 for(i=0;i<paramNum;i++)buf[i+1]=*(u32*)((vlist+=4)-4);
 va_end(vlist);
 DC_StoreRange(buf,0x100);
 result=PXI_SendWordByFifo(10,(u32)buf,FALSE);
 OS_SendMessage(&_021E4228,buf,1);
 if(result<0)return 8;
 return 2;
}

#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern u8 *mbc;
extern u32 _02247480;
extern u16 *WM_ReadMPData(void*,u16);
extern void MBi_CommParentRecvDataPerChild(void*,u16);
void MBi_CommParentRecvData(void*);
void MBi_CommParentRecvData(void*data){u16 i;for(i=0;i<16;i++){u8 *entry=mbc+i*0x5d4;if(entry[0x1d52])*(u16*)(entry+0x1d4a)=0;}_02247480=0;for(i=1;i<=15;i++){u16*p=WM_ReadMPData(data,i);if(p&&*p!=65535&&*p!=0)MBi_CommParentRecvDataPerChild(p,i);}}

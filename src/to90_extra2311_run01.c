#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u16 MATH_CalcCRC16(const MATHCRC16Table *,const void *,u32);
extern u32 _021D43C8[];
extern void writeNvram(u32,u32,void *);
extern int verify(void *,u32,u32,void *),writeDisable(void);
int DWCi_BACKUPlWritePage(u8 *,int *,void *),DWCi_BACKUPlWriteAll(u8 *);
int DWCi_BACKUPlWritePage(u8 *p,int *flags,void *scratch){int address=_021D43C8[2];int i;for(i=0;i<4;i++,p+=256,address+=256){if(flags[i]){do{writeNvram(address,256,p);}while(!verify(p,address,256,scratch));}}return writeDisable()!=0;}

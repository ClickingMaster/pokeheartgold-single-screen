#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 _021E370C[];extern volatile u32 _021E3728;extern void PXIi_HandlerRecvFifoNotEmpty(void);void PMi_WaitBusy(void);
void PMi_WaitBusy(void){volatile u32*busy=&_021E3728;if(!*(u32*)(_021E370C+0x1c))return;do{if(OS_GetCpsrIrq()==0x80)PXIi_HandlerRecvFifoNotEmpty();}while(*busy);}

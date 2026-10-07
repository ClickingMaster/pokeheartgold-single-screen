#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_strength_reduction off
extern u8 _021E370C[],_021E3750[],_021E3738[];
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define W(p,n) (*(int*)((u8*)(p)+(n)))
void PMi_CommonCallback(PXIFifoTag,u32,BOOL);
void PM_Init(void){int i;if(H(_021E370C,0))return;H(_021E370C,0)=1;W(_021E370C,28)=0;W(_021E370C,32)=0;PXI_Init();while(!PXI_IsCallbackReady(8,PXI_PROC_ARM7)){}PXI_SetFifoRecvCallback(8,PMi_CommonCallback);for(i=0;i<5;i++)H(_021E3750,i*8)=0;OS_InitMutex((OSMutex*)_021E3738);W(_021E370C,16)=*(int*)0x027ffc3c;}

#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
extern char _021E4D60[],_021E4E40[];
extern u16 CTRDGi_Work[];
extern void CTRDGi_InitCommon(void),CTRDGi_InitModuleInfo(void),CTRDGi_InitTaskThread(void*),CTRDGi_SendtoPxi(int);
extern void CTRDGi_CallbackForInitModuleInfo(PXIFifoTag,u32,BOOL),CTRDGi_PulledOutCallback(PXIFifoTag,u32,BOOL),CTRDGi_CallbackForSetPhi(PXIFifoTag,u32,BOOL);
extern void CTRDG_TerminateForPulledOut(void);
extern void CTRDG_Enable(BOOL);

void CTRDGi_CallbackForInitModuleInfo(PXIFifoTag tag,u32 data,BOOL error){if((data&63)==1)CTRDGi_Work[0]=1;else OS_Terminate();}

void CTRDGi_PulledOutCallback(PXIFifoTag tag,u32 data,BOOL error){int result;int(*cb)(void);if((data&63)==17){if(W(_021E4D60,12))return;cb=(int(*)(void))P(_021E4D60,24);result=0;if(cb)result=cb();if(result)CTRDG_TerminateForPulledOut();W(_021E4D60,12)=1;}else OS_Terminate();}

void CTRDG_TerminateForPulledOut(void){CTRDGi_SendtoPxi(2);OS_Terminate();}

void CTRDGi_CallbackForSetPhi(PXIFifoTag tag,u32 data,BOOL error){W(_021E4D60,4)=0;}

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

void CTRDG_Init(void){if(W(_021E4D60,8))return;W(_021E4D60,8)=1;CTRDGi_InitCommon();W(_021E4D60,12)=0;PXI_Init();while(!PXI_IsCallbackReady((PXIFifoTag)13,(PXIProc)1)){}PXI_SetFifoRecvCallback((PXIFifoTag)13,CTRDGi_CallbackForInitModuleInfo);CTRDGi_InitModuleInfo();PXI_SetFifoRecvCallback((PXIFifoTag)13,0);PXI_SetFifoRecvCallback((PXIFifoTag)13,CTRDGi_PulledOutCallback);P(_021E4D60,24)=0;CTRDGi_InitTaskThread(_021E4E40);PXI_SetFifoRecvCallback((PXIFifoTag)17,CTRDGi_CallbackForSetPhi);CTRDG_Enable(0);}

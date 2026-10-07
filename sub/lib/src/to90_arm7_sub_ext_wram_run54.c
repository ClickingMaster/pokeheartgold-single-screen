#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern u8 wmspW[];
extern u16*__VENEER_WMSP_GetBuffer4Callback2Wm9(void);
extern void __VENEER_WMSP_ReturnResult2Wm9(void*);
extern u32 __VENEER_OS_DisableInterrupts(void);
extern void __VENEER_OS_RestoreInterrupts(u32);
extern void __VENEER_MI_CpuCopy8(const void*,void*,u32),__VENEER_MI_CpuFill8(void*,int,u32),__VENEER_MIi_CpuClear16(int,void*,u32);
extern void WMSP_CancelVAlarm(void),WMSP_SetThreadPriorityLow(void),WMSP_CleanSendQueue(u16),WmspError_06005328(u16,u16);extern u16*WMSP_WL_ParamSetNullKeyResponseMode(void*,int),*WMSP_WL_MaClearData(void*,int);
void WMSP_EndMP(void*r);
// @fn WMSP_EndMP
void WMSP_EndMP(void*r){u8 buf[512];int clean=0;u32 irq;u8*s=P(wmspW+0x1000,0x550);u16*p,err;if(H(s,0)!=9&&H(s,0)!=10){p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=16;p[1]=3;__VENEER_WMSP_ReturnResult2Wm9(p);return;}irq=__VENEER_OS_DisableInterrupts();if(W(s,12)==1)clean=1;W(s,12)=0;WMSP_CancelVAlarm();WMSP_SetThreadPriorityLow();if(H(s,0)==10)H(s,0)=8;else if(H(s,0)==9)H(s,0)=7;__VENEER_OS_RestoreInterrupts(irq);err=WMSP_WL_ParamSetNullKeyResponseMode(buf,0)[2];if(err){WmspError_06005328(0x216,err);return;}err=WMSP_WL_MaClearData(buf,7)[2];if(err){WmspError_06005328(0x104,err);return;}H(s,0x8a)=0;if(clean)WMSP_CleanSendQueue(0xffff);p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=16;p[1]=0;__VENEER_WMSP_ReturnResult2Wm9(p);}

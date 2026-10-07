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
extern u16*WMSP_WL_MaClearData(void*,int);
void WMSP_EndDCF(void*r);
// @fn WMSP_EndDCF
void WMSP_EndDCF(void*r){u8 buf[512];u8*s=P(wmspW+0x1000,0x550);u32 irq=__VENEER_OS_DisableInterrupts();u16*p,err;if(H(s,0)!=11){__VENEER_OS_RestoreInterrupts(irq);p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=19;p[1]=3;__VENEER_WMSP_ReturnResult2Wm9(p);}else{W(s,0x10)=0;H(s,0)=8;__VENEER_OS_RestoreInterrupts(irq);err=WMSP_WL_MaClearData(buf,7)[2];if(err){p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=19;p[1]=1;p[2]=0x104;p[3]=err;__VENEER_WMSP_ReturnResult2Wm9(p);}else{p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=19;p[1]=0;__VENEER_WMSP_ReturnResult2Wm9(p);}}}

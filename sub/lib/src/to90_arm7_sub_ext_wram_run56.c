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

void WMSP_StartDCF(u32*r);
// @fn WMSP_StartDCF
void WMSP_StartDCF(u32*r){u8*s=P(wmspW+0x1000,0x550);u32 raw=r[2];u8*buffer=(void*)r[1];u16 size=raw;u32 irq=__VENEER_OS_DisableInterrupts();P(s,0xb0)=buffer;H(s,0xb8)=size;P(s,0xb4)=buffer+size;H(s,0xae)=0;W(s,0xa8)=0;H(s,0xac)=0;W(s,0x18)=0;H(s,0)=11;u16*p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=17;p[1]=0;p[2]=14;__VENEER_WMSP_ReturnResult2Wm9(p);W(s,0x10)=1;__VENEER_OS_RestoreInterrupts(irq);}

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
extern u16*WMSP_WL_DevIdle(void*),*WMSP_WL_ParamSetPreambleType(void*,int);extern void WmspError_06003FF0(u16,u16);
void WMSP_EndScan(void*r);
// @fn WMSP_EndScan
void WMSP_EndScan(void*r){u8 buf[512];void*b;u8*s;u16*p,err;s=P(wmspW+0x1000,0x550);b=buf;if(H(s,0)!=5){p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=11;p[1]=3;__VENEER_WMSP_ReturnResult2Wm9(p);}else{err=WMSP_WL_DevIdle(b)[2];if(err){WmspError_06003FF0(0x302,err);return;}H(s,0)=2;if(!H(s,0x1ee)){err=WMSP_WL_ParamSetPreambleType(b,1)[2];if(err){WmspError_06003FF0(0x20e,err);return;}H(s,0x1ee)=1;}p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=11;p[1]=0;__VENEER_WMSP_ReturnResult2Wm9(p);}}

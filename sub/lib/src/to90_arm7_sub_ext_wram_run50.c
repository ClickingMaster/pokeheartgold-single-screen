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
extern int WMSP_DisconnectCore(void*,int,u16*);
void WMSP_Disconnect(u32*r);
// @fn WMSP_Disconnect
void WMSP_Disconnect(u32*r){u16 result;u16 mask=r[1];if(WMSP_DisconnectCore(r,0,&result)==1){u16*p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=13;p[1]=0;p[4]=mask;p[5]=result;__VENEER_WMSP_ReturnResult2Wm9(p);}}

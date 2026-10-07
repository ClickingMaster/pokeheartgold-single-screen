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
extern u16*WMSP_WL_MaData(void*,void*);
void WMSP_SetDCFData(u32*r);
// @fn WMSP_SetDCFData
void WMSP_SetDCFData(u32*r){u8 packet[48],buf[512];u8*s=P(wmspW+0x1000,0x550);__VENEER_MI_CpuCopy8(r+1,s+0xa2,6);W(s,0xa8)=r[3];H(s,0xac)=r[4];W(s,0x18)=1;__VENEER_MIi_CpuClear16(0,packet,48);H(packet,0)=0;H(packet,6)=r[4];packet[14]=H(s,0x1ec)==2?20:10;__VENEER_MI_CpuCopy8(r+1,packet+24,6);__VENEER_MI_CpuCopy8(s+0xe0,packet+30,6);W(packet,44)=r[3];u16*out=WMSP_WL_MaData(buf,packet);u16*p=__VENEER_WMSP_GetBuffer4Callback2Wm9();p[0]=18;if(out[2]==0)p[1]=0;else p[1]=1;if(out[2]){p[2]=0x100;p[3]=out[2];}__VENEER_WMSP_ReturnResult2Wm9(p);}

#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define P(p,o) (*(u8**)((u8*)(p)+(o)))
extern u8* WMi_GetSystemWork(void);
extern int WMi_CheckStateEx(int,...);
int WM_GetMPReceiveBufferSize(void);
int WM_GetMPReceiveBufferSize(void) {u8*p=WMi_GetSystemWork(),*s;int parent,size;if(WMi_CheckStateEx(2,7,8))return 0;DC_InvalidateRange(P(p,4)+12,4);s=P(p,4);if(W(s,12)==1)return 0;DC_InvalidateRange(s+0x188,2);s=P(p,4);parent=H(s,0x188)==0;DC_InvalidateRange(s+0x3e,2);s=P(p,4);size=H(s,0x3e);if(parent==1){DC_InvalidateRange(s+0xf8,2);return (((size+12)*H(P(p,4),0xf8)+0x29)&~31)*2;}else return ((size+0x51)&~31)*2;}

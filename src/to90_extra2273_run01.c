#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern const u32 ov08_02225CF8[];void ov08_02224BCC(void*,u32);void ov08_02224B98(void*,u32);
void ov08_02224134(u8*w,int mode);
void ov08_02224134(u8*w,int mode){ov08_02224BCC(P(w,0x34),ov08_02225CF8[mode]);switch(mode){case 0:ov08_02224B98(P(w,0x34),B(w,0x114d));break;case 1:ov08_02224B98(P(w,0x34),B((u8*)P(w,0)+B(w,0x114d),0x27));break;case 2:break;}}

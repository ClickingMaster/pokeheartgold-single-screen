#pragma force_active on
#include "global.h"
#pragma force_active on
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
extern void BeginNormalPaletteFade(int,int,int,u16,int,int,int);
void ov74_0222D7F0(void *,int,int,int *);
void ov74_0222D7F0(void *state,int mode,int next,int *status) { BeginNormalPaletteFade(0,mode,mode,0,6,1,85); if(status) *status=28; U32(state,0x2c40)=next; }

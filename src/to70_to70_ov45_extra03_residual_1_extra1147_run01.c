#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
void *ov45_02238C08(void *);
int ov45_02238B70(u8 *,u8 *,u32 *);
void ov45_02238C1C(u32 *,u8 *);
extern void ov45_022377AC(void *,u32);
extern void *ov45_02231C78(void *);
void *ov45_02238C08(void *p){if(W(p,4)==0)return 0;return (void*)W(p,0);}
void ov45_02238C1C(u32 *v,u8 *p){u32 len;len=0;ov45_02238B70(p,0,&len);{u32 n=len;u32 zero=0;v[0]=zero;v[1]=zero;v[2]=zero;ov45_022377AC(v,n);}ov45_02238B70(p,ov45_02231C78(v),&len);}

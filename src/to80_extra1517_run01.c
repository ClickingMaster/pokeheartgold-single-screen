#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void ManagedSprite_SetDrawFlag(void *,int); extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,int,int,int);
void ov15_022002B4(void *ctx,int selected);
void ov15_022002B4(void *ctx,int selected){int i;u8 *p=ctx;for(i=0;i<6;i++,p+=4){if(selected!=i)ManagedSprite_SetDrawFlag(P(p,0x254),0);else ManagedSprite_SetPositionXYWithSubscreenOffset(P(p,0x254),86,76,0x100000);}}

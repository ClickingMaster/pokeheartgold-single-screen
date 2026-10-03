#pragma force_active on
#include "global.h"
extern s16 ov07_02222674(int,int,int);extern void Pokepic_SetAttr(void *,int,int);extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);extern void ManagedSprite_SetPositionXY(void *,s16,s16);
void ov07_022226C4(void *,int,int,int,int),ov07_022226FC(void *,int,int,int,int);
#pragma force_active on
void ov07_022226FC(void *sprite,int y,int amount,int phase,int reverse){s16 x,oldy;int v=ov07_02222674(y,amount,phase);if(reverse==1){v=(s16)(~0u*v);y=(s16)(y-amount);}ManagedSprite_GetPositionXY(sprite,&x,&oldy);ManagedSprite_SetPositionXY(sprite,x,(s16)(y+v));}

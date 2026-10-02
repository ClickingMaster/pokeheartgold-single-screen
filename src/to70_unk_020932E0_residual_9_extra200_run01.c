#pragma force_active on
#include "global.h"
#pragma force_active on
void System_GetTouchHeldCoords(u32 *,u32 *);
void Sprite_SetMatrix(void *,const VecFx32 *);
void sub_020949F4(u8 *);
#define W(p,o) (*(u32 *)((p)+(o)))
#define P(p,o) (*(void **)((p)+(o)))
void sub_020949F4(u8 *w){
 u32 x,y;
 VecFx32 pos,copy;
 if(W(w,0x4680)!=0){
 System_GetTouchHeldCoords(&x,&y);pos.x=x<<12;pos.y=y<<12;pos.z=0;
 copy=pos;copy.y-=0x4000;
 Sprite_SetMatrix(*(void **)((u8*)P(w,0x46a0)+W(w,0x4684)*52),&copy);
 Sprite_SetMatrix(P(w,0x8c0),&pos);
 W(w,0x4694)=x;W(w,0x4698)=y;
 }
}

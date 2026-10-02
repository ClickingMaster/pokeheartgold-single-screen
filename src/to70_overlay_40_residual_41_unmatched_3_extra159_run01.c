#pragma force_active on
#include "global.h"
#pragma force_active on
void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
void ov40_022441F8(u8 *);
#define W(p,o) (*(int *)((p)+(o)))
#define P(p,o) (*(void **)((p)+(o)))
void ov40_022441F8(u8 *w){
 int i,dx,dy;
 u8 *p;
 s16 y,x;
 i=0;
 if(W(w,0x204)>0){p=w;
 do{
  if(i>=W(w,0x210)&&i<W(w,0x214)){dx=16;dy=16;}else{dx=4;dy=8;}
  ManagedSprite_GetPositionXY(P(p,12),&x,&y);
  ((u8*)P(p,16))[0]=y-dy;((u8*)P(p,16))[2]=x-dx;
  ((u8*)P(p,16))[1]=y+dy;((u8*)P(p,16))[3]=x+dx;
  if(i==0)((u8*)P(p,16))[2]-=4;
 i++;p+=28;
 }while(i<W(w,0x204));
 }
}

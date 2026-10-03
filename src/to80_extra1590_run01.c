#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Pair {s16 y,x;} Pair;
typedef struct State {u32 heap;void *a,*b,*course,*sprites[9];u32 extra[3];unsigned index:2;unsigned rest:30;unsigned low:10;unsigned state:6;unsigned middle:8;unsigned count:8;} State;
extern void ManagedSprite_ResetSpriteAnimCtrlState(void *),ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_SetAnimateFlag(void *,int);
extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *),ManagedSprite_GetPositionXYWithSubscreenOffset(void *,s16 *,s16 *,int);
extern void ManagedSprite_SetPositionXY(void *,s16,s16),ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int);
extern void PlaySE(int),GF_AssertFail(void);
extern int ov96_021E5F24(void *);
extern void ov96_0220C54C(State *,int,u8,u8,int),ov96_0220C90C(State *,int,s16,s16),ov96_0220C578(State *,int);
void ov96_0220B9A8(State *);
void ov96_0220B9A8(State *s){Pair p2,p3,p4; s16 delta; s8 index;void *tmp;
 switch(s->state){
 case 0:ManagedSprite_ResetSpriteAnimCtrlState(s->sprites[3]);ManagedSprite_SetDrawFlag(s->sprites[3],1);PlaySE(0x8b4);s->state++;break;
 case 1:if(++s->count>=2){ManagedSprite_ResetSpriteAnimCtrlState(s->sprites[4]);ManagedSprite_SetAnimateFlag(s->sprites[4],0);ManagedSprite_SetDrawFlag(s->sprites[4],1);ManagedSprite_SetDrawFlag(s->sprites[0],0);ManagedSprite_SetDrawFlag(s->sprites[1],0);ManagedSprite_SetDrawFlag(s->sprites[2],0);s->count=0;s->state++;}break;
 case 2:ManagedSprite_GetPositionXY(s->sprites[4],&p2.x,&p2.y);delta=(-64-p2.y)/4;
  if(p2.y<=-52||delta==0){index=(s8)(s->index-1);if(index<0)index=2;ov96_0220C54C(s,6,(u8)ov96_021E5F24(s->course),(u8)index,0);p2.x=40;p2.y=-40;ov96_0220C90C(s,6,40,-40);s->state++;}
  else{ManagedSprite_SetPositionXYWithSubscreenOffset(s->sprites[4],p2.x,(s16)(p2.y+delta),0x100000);ManagedSprite_GetPositionXYWithSubscreenOffset(s->sprites[6],&p2.x,&p2.y,0x100000);{int dist=272-p2.y;if(dist<0)dist=-dist;p2.y+=dist>>2;}p2.y=p2.y>216?216:p2.y;ov96_0220C90C(s,6,p2.x,p2.y);}break;
 case 3:ManagedSprite_GetPositionXYWithSubscreenOffset(s->sprites[5],&p3.x,&p3.y,0x100000);{int dist=136-p3.y;if(dist<0)dist=-dist;delta=(s16)(dist>>1);}if(p3.y<136){if(delta==0)delta=1;p3.y+=delta;ov96_0220C90C(s,5,p3.x,p3.y);ManagedSprite_GetPositionXYWithSubscreenOffset(s->sprites[6],&p3.x,&p3.y,0x100000);p3.y+=delta;ov96_0220C90C(s,6,p3.x,p3.y);}
  if(++s->count>=4){tmp=s->sprites[5];s->sprites[5]=s->sprites[6];s->sprites[6]=tmp;tmp=s->sprites[7];s->sprites[7]=s->sprites[8];s->sprites[8]=tmp;ov96_0220C90C(s,5,40,48);ov96_0220C90C(s,6,40,136);ManagedSprite_SetPositionXYWithSubscreenOffset(s->sprites[4],136,-8,0x100000);PlaySE(0x8b5);s->count=0;s->state++;}break;
 case 4:ManagedSprite_GetPositionXY(s->sprites[4],&p4.x,&p4.y);{int dist=-24-p4.y;if(dist<0)dist=-dist;delta=(s16)dist;}if(delta!=0)delta/=2;p4.y+=delta+2;if(p4.y>=112){PlaySE(0x8b6);ManagedSprite_SetPositionXY(s->sprites[4],136,112);ManagedSprite_SetAnimateFlag(s->sprites[4],1);ManagedSprite_ResetSpriteAnimCtrlState(s->sprites[3]);s->state++;}else ManagedSprite_SetPositionXYWithSubscreenOffset(s->sprites[4],p4.x,p4.y,0x100000);break;
 case 5:if(++s->count>=2){ManagedSprite_SetDrawFlag(s->sprites[4],0);ov96_0220C54C(s,0,(u8)ov96_021E5F24(s->course),(u8)s->index,1);s->count=0;s->state++;}break;
 case 6:ov96_0220C578(s,0);break;
 default:GF_AssertFail();break;
 }
}

#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Bits {u32 low:8,state:4,mid:2,kind:2,rest:16;} Bits;
typedef struct S {void *base;u32 a,b;void *back,*front;u8 gap[0xce];s8 count;u8 pad;Bits flags;} S;
typedef struct Out {u32 low:17,value:2,high:13;} Out;
extern void ManagedSprite_SetPositionXYWithSubscreenOffset(void *,s16,s16,int);
extern void ManagedSprite_SetDrawFlag(void *,int);
extern void ManagedSprite_ResetSpriteAnimCtrlState(void *);
extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *);
extern void ov96_021EAB38(void *,int);
extern void ov96_022193CC(int,int);
extern int ov96_02219794(void *,void *,u8);
extern int ov96_02219940(void *,void *,u8);
void ov96_02216770(S *,s16 *,void *,void *,Out *,int,int);
void ov96_02216770(S *s,s16 *p,void *a,void *b,Out *out,int second,int first){
 s16 x,y;
 switch(s->flags.state){
 case 0:ManagedSprite_SetPositionXYWithSubscreenOffset(s->front,p[0],p[1],0x1e0000);ManagedSprite_SetDrawFlag(s->front,1);ManagedSprite_ResetSpriteAnimCtrlState(s->front);ManagedSprite_SetPositionXYWithSubscreenOffset(s->back,p[0],p[1],0x1e0000);ManagedSprite_SetDrawFlag(s->back,1);ov96_021EAB38(s->base,0);s->count=0;ov96_022193CC(0x8b4,first);s->flags.state++;break;
 case 1:ManagedSprite_GetPositionXY(s->back,&x,&y);y-=40;ManagedSprite_SetPositionXYWithSubscreenOffset(s->back,x,y,0x1e0000);s->count++;if(s->count>=10){ManagedSprite_SetDrawFlag(s->back,0);s->flags.state++;}break;
 case 2:if(first){if(ov96_02219794(a,b,s->flags.kind)){out->value=1;s->flags.state++;}}else if(second){if(ov96_02219940(a,b,s->flags.kind)){out->value=1;s->flags.state++;}}else{s->flags.state++;}break;
 case 3:break;
 default:GF_AssertFail();break;
 }
}

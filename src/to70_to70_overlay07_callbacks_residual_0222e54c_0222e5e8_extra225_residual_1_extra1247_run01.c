#pragma force_active on
#include "global.h"
typedef struct{u32 a;void *ctx,*pic;s16 x,y;u8 p[0x44];volatile int mode;} Work;
extern Work *BattleAnimUtil_AllocWork(void *,int);extern int ov07_0221C468(void *),ov07_0221C470(void *),ov07_02222004(void *,int),Pokepic_GetAttr(void *,int);
extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221FA48(void *,int);extern void Pokepic_SetAttr(void *,int,int),ov07_0222E3A4(Work *,int),ov07_0221C410(void *,void (*)(void),Work *),ov07_0222E4F8(void);
void ov07_0222E54C(void *);
void ov07_0222E54C(void *ctx){Work *w=BattleAnimUtil_AllocWork(ctx,88);int side;w->ctx=ctx;side=ov07_02222004(w->ctx,ov07_0221C468(ctx));if(((s32)ov07_0221C4A8(ctx, 0))==0){w->pic=ov07_0221FA48(w->ctx,ov07_0221C468(ctx));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);}else{w->pic=ov07_0221FA48(w->ctx,ov07_0221C470(ctx));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);}Pokepic_SetAttr(w->pic,11,50);{int mode;switch(side){case 1:mode=0;break;default:mode=1;break;}w->mode=mode;}ov07_0222E3A4(w,w->mode);ov07_0221C410(w->ctx,ov07_0222E4F8,w);}

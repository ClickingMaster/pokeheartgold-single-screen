#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void*BattleAnimUtil_AllocWork(void*,int);extern int ov07_02225AE0(void*,int),ov07_0221FAA0(void*,int),Pokepic_GetAttr(void*,int);
extern u32 ov07_0221C4A8(void *p, int i);extern void BattleAnimUtil_InitCommonWork(void*,void*),BattleAnimUtil_CollectPicsByFlags(void*,int,void*,int*),Pokepic_SetVisible(void*,int,int,int,int),Pokepic_SetAttr(void*,int,int),ov07_0221C410(void*,void*,void*),ov07_02225C50(void*),ov07_02225BC4(void*),ov07_02225B38(void*);void ov07_02225CC4(void*);
void ov07_02225CC4(void*ctx){void*w=BattleAnimUtil_AllocWork(ctx,80);int n,tmp;BattleAnimUtil_InitCommonWork(ctx,w);H(w,28)=((s32)ov07_0221C4A8(ctx, 3));W(w,32)=((s32)ov07_0221C4A8(ctx, 4));n=((s32)ov07_0221C4A8(ctx, 0));BattleAnimUtil_CollectPicsByFlags(ctx,n,(u8*)w+40,&tmp);n=ov07_02225AE0(ctx,n);if(((s32)ov07_0221C4A8(ctx, 5))){W(w,60)=80-ov07_0221FAA0(ctx,n);W(w,64)=0;Pokepic_SetVisible(P(w,48),0,W(w,60),80,0);ov07_0221C410(P(w,4),ov07_02225C50,w);}else{W(w,68)=Pokepic_GetAttr(P(w,48),1);W(w,72)=W(w,68);W(w,60)=80-ov07_0221FAA0(ctx,n);W(w,64)=W(w,60);if(*(s16*)((u8*)w+28)>0){Pokepic_SetVisible(P(w,48),0,0,80,W(w,60));ov07_0221C410(P(w,4),ov07_02225BC4,w);}else{W(w,60)=0;W(w,68)+=W(w,64);Pokepic_SetAttr(P(w,48),1,W(w,68));Pokepic_SetVisible(P(w,48),0,0,80,W(w,60));ov07_0221C410(P(w,4),ov07_02225B38,w);}}}

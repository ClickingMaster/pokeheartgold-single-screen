#pragma force_active on
#include "global.h"
#pragma force_active on
int ov07_0221FAC8(void *,int);
extern u32 ov07_0221C4A8(void *p, int i);void BattleAnimUtil_CollectPicsByFlags(void *,int,void *,int *),Pokepic_SetAttr(void *,int,int),ov07_022248A0(void *);
void *BattleAnimUtil_AllocWork(void *,int),*ov07_0221C514(void *),*ov07_0221C4E8(void *,int);void ManagedSprite_SetOamMode(void *,int),BattleAnimUtil_SetBlendWithBattleBg(void *,int,int),ov07_02222AC4(void *,s16,s16,s16,s16,int),ov07_0221C410(void *,void *,void *),ov07_022247C8(void),ov07_022247F4(void *);
void ov07_022247F4(void *w){void **s=BattleAnimUtil_AllocWork(w,48);int mask,i,x,y,z,extra,final;s[0]=w;s[1]=ov07_0221C514(w);mask=((s32)ov07_0221C4A8(w, 0));for(i=0;i<4;i++)if(mask&(1<<i))ManagedSprite_SetOamMode(ov07_0221C4E8(s[0],i),1);x=((s32)ov07_0221C4A8(w, 1));z=((s32)ov07_0221C4A8(w, 3));BattleAnimUtil_SetBlendWithBattleBg(s[0],x,z);y=((s32)ov07_0221C4A8(w, 2));extra=((s32)ov07_0221C4A8(w, 4));final=((s32)ov07_0221C4A8(w, 5));ov07_02222AC4(s+2,(s16)x,(s16)y,(s16)z,(s16)extra,final);ov07_0221C410(s[0],ov07_022247C8,s);}
void ov07_022248A0(void *w){int i;int arg=((s32)ov07_0221C4A8(w, 1)),n;int data[5];BattleAnimUtil_CollectPicsByFlags(w,((s32)ov07_0221C4A8(w, 0)),data,&n);i=0;if(n>0){do{if(ov07_0221FAC8(w,data[4])!=1||arg)Pokepic_SetAttr((void *)data[2],6,arg);i++;}while(i<n);}}

#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*obj;int pad;void *pic;int y,start,pad2[2],active,pad3[3];int handle;} State;
extern State *BattleAnimUtil_AllocWork(void *,int);extern int ov07_0221C470(void *),Pokepic_GetAttr(void *,int),ov07_0221FAC8(void *,int),ov07_02222D88(u16,u16),ov07_0221BFD0(void *),BattleAnimUtil_GetBattlerType(void *,int),ov07_0221FAEC(void *,int),ov07_0221FAE8(void *);
extern void *ov07_0221FA48(void *,int),*ov07_02222D90(void),*ov07_02222BE4(void *,int,int);
extern void Pokepic_SetAttr(void *,int,int),ov07_0221FAF8(void *,int),SetBgPriority(u8,u8),ov07_0221C410(void *,void *,void *);extern void ov07_022302A8(void);
void ov07_022303A4(void *);
void ov07_022303A4(void *owner){State *s=BattleAnimUtil_AllocWork(owner,52);s16 x;int y;void *manager;int priority;s->owner=owner;s->pic=ov07_0221FA48(s->owner,ov07_0221C470(owner));x=Pokepic_GetAttr(s->pic,0);y=(s16)Pokepic_GetAttr(s->pic,1);y=(s16)(y-Pokepic_GetAttr(s->pic,41));if(ov07_0221FAC8(s->owner,ov07_0221C470(s->owner))==0)Pokepic_SetAttr(s->pic,6,1);x-=40;s->start=y;y-=40;s->y=y;s->handle=ov07_02222D88(-x,-y);ov07_0221FAF8(s->owner,1);manager=ov07_02222D90();s->obj=ov07_02222BE4(manager,s->handle,ov07_0221BFD0(s->owner));s->active=1;{int n=BattleAnimUtil_GetBattlerType(s->owner,ov07_0221C470(owner));if(n==3||n==4){priority=ov07_0221FAEC(s->owner,1);SetBgPriority(priority,ov07_0221FAE8(s->owner));SetBgPriority(0,ov07_0221FAE8(s->owner)+1);}}ov07_0221C410(s->owner,ov07_022302A8,s);}

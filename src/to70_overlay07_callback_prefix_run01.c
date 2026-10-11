#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *BattleAnimUtil_AllocWork(void *,int),*BattleAnimSystem_GetBattlerPokepic(void *,int),*SpriteSystem_NewSprite(void *,void *,void *),*BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(void *,void *),void *);
extern u16 BattleAnimSystem_GetAttacker(void *);
extern int Pokepic_GetAttr(void *,int),ov07_0221FAA0(void *,int),ov07_0221FAF8(void *,int),ov07_0221FAE8(void *),ov07_0221BFC0(void *);
extern u32 BattleAnimSystem_GetArg(void *p, int i);
extern void BattleAnimUtil_SetBlendWithMask(void *,int,int,int),BattleAnimSystem_GetSpriteTemplate(void *,void *),ManagedSprite_SetOamMode(void *,int),ManagedSprite_SetPriority(void *,int),ManagedSprite_SetPositionXY(void *,s16,s16),ov07_022297B8(void *,void *);
typedef struct {void *sprite;u8 padding[0x48];} Entry;
typedef struct {u8 state,other,pad[2];int low,high,padc;void *owner,*sys,*manager,*pic;s16 attr,pad22;int unknown;u8 pad28[8];Entry entries[2];} Work;
void ov07_02229BFC(void *,void *,void *,void *);
void ov07_02229BFC(void *owner,void *sys,void *manager,void *sprite) {
 Work *w=BattleAnimUtil_AllocWork(owner,0xf0);int a,b,i;u8 template[0x34];
 w->state=0;w->other=0;w->owner=owner;w->manager=manager;w->sys=sys;w->low=0;w->high=16;
 w->pic=BattleAnimSystem_GetBattlerPokepic(w->owner,BattleAnimSystem_GetAttacker(w->owner));w->attr=Pokepic_GetAttr(w->pic,1);
 w->unknown=ov07_0221FAA0(w->owner,BattleAnimSystem_GetAttacker(w->owner));
 a=ov07_0221FAF8(owner,2);b=ov07_0221FAF8(owner,1);
 BattleAnimUtil_SetBlendWithMask(w->owner,(1<<a)|(1<<b)|1,-1,-1);
 *(volatile u16 *)0x04000052=w->low|(w->high<<8);
 BattleAnimSystem_GetSpriteTemplate(template,owner);w->entries[0].sprite=sprite;
 for(i=1;i<((s32)BattleAnimSystem_GetArg(w->owner, 0));i++)w->entries[i].sprite=SpriteSystem_NewSprite(w->sys,w->manager,template);
 for(i=0;i<((s32)BattleAnimSystem_GetArg(w->owner, 0));i++){ManagedSprite_SetOamMode(w->entries[i].sprite,1);ManagedSprite_SetPriority(w->entries[i].sprite,ov07_0221FAE8(w->owner)+1);}
 if(ov07_0221BFC0(owner)==1){ManagedSprite_SetPositionXY(w->entries[0].sprite,160,54);ManagedSprite_SetPositionXY(w->entries[1].sprite,80,39);}else{ManagedSprite_SetPositionXY(w->entries[0].sprite,100,54);ManagedSprite_SetPositionXY(w->entries[1].sprite,180,39);}
 BattleAnimSystem_CreateTaskDefaultPriority(w->owner,ov07_022297B8,w);
}

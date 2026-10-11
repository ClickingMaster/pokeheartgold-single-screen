#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct SpriteWrap {void*s;} SpriteWrap;
typedef struct Work {u8 state,pad[3];void*owner,*p8,*system;u8 pad10[16];s16 x,y;void*pic;SpriteWrap*first;u32 p2c;SpriteWrap*second;u8 motion1[36],motion2[36],motion3[36];u8 counter,flag,a,b;s16 delta;u16 padend;} Work;
extern Work *BattleAnimUtil_AllocWork(void*,u32);
extern void BattleAnimUtil_InitCommonWork(void*,Work*),BattleAnimUtil_SetBlendWithBattleBg(void*,int,int),BattleAnimSystem_CreateTaskDefaultPriority(void*,void(*)(void*,Work*),Work*),BattleAnimSystem_DestroyTask(void*,void*),BattleAnimUtil_FreeWork(void*);
extern int BattleAnimSystem_GetAttacker(void*),ov07_0221FA80(void*,int),ov07_0221FA90(void*,int),ov07_0221FAE8(void*),BattleAnimSystem_GetHeapID(void*);
extern void *BattleAnimSystem_GetBattlerPokepic(void*,int),*BattleAnimSystem_GetPaletteData(void*);
extern SpriteWrap *BattleAnimSystem_GetBattlerSprite(void*,int);
extern int Pokepic_GetAttr(void*,int);
extern void ManagedSprite_SetPriority(void*,int),ManagedSprite_SetOamMode(void*,int),ManagedSprite_TickFrame(void*),SpriteSystem_DrawSprites(void*);
extern void*Sprite_GetPaletteProxy(void*);extern u32 ObjPlttTransfer_GetPaletteVramOffset(void*,int);
extern void PaletteData_LoadNarc_CustomTint(void*,int,int,int,int,u32,u16,int,int,int);
extern void ov07_02222268(void*,s16,s16,s16,s16,u16);
extern int ov07_022222F0(void*,void*),ov07_02222314(void*,void*);
void ov07_0222AFAC(void*,Work*);void ov07_0222B130(void*);
void ov07_0222B130(void*owner){Work*w=BattleAnimUtil_AllocWork(owner,168);BattleAnimUtil_InitCommonWork(owner,w);w->flag=0;w->a=8;w->b=8;BattleAnimUtil_SetBlendWithBattleBg(w->owner,-1,-1);*(volatile u16*)0x4000052=w->a|(w->b<<8);w->pic=BattleAnimSystem_GetBattlerPokepic(w->owner,BattleAnimSystem_GetAttacker(w->owner));w->x=Pokepic_GetAttr(w->pic,0);w->y=Pokepic_GetAttr(w->pic,1);w->first=BattleAnimSystem_GetBattlerSprite(w->owner,0);w->second=BattleAnimSystem_GetBattlerSprite(w->owner,1);w->delta=-Pokepic_GetAttr(w->pic,41);int member=ov07_0221FA80(w->owner,BattleAnimSystem_GetAttacker(w->owner));int narc=ov07_0221FA90(w->owner,BattleAnimSystem_GetAttacker(w->owner));SpriteWrap*s=w->first;ManagedSprite_SetPriority(s,ov07_0221FAE8(w->owner)+1);u32 offset=ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(s->s),1);void*pd=BattleAnimSystem_GetPaletteData(w->owner);PaletteData_LoadNarc_CustomTint(pd,narc,member,BattleAnimSystem_GetHeapID(w->owner),2,32,(u16)(offset<<4),196,196,196);ManagedSprite_SetOamMode(s,1);s=w->second;ManagedSprite_SetPriority(s,ov07_0221FAE8(w->owner)+1);offset=ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(s->s),1);pd=BattleAnimSystem_GetPaletteData(w->owner);PaletteData_LoadNarc_CustomTint(pd,narc,member,BattleAnimSystem_GetHeapID(w->owner),2,32,(u16)(offset<<4),196,196,196);ManagedSprite_SetOamMode(s,1);BattleAnimSystem_CreateTaskDefaultPriority(w->owner,ov07_0222AFAC,w);}

#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Template {s16 x,y,z,anim;int priority,palette,vram;int resources[6];int extra[2];} Template;
typedef struct State {void *ctx,*system,*manager;u8 pad[12];void *pic;u8 pad1c[40];s16 y,attr;void *sprites[6];u8 pad60[24];int priority[6];} State;
void *BattleAnimUtil_AllocWork(void *,int);int BattleAnimSystem_GetAttacker(void *);void *BattleAnimSystem_GetBattlerPokepic(void *,int);int Pokepic_GetAttr(void *,int);int ov07_0221FAA0(void *,int);
void BattleAnimSystem_GetSpriteTemplate(Template *,void *);void *SpriteSystem_NewSprite(void *,void *,Template *);void ManagedSprite_SetDrawFlag(void *,int);
void ManagedSprite_SetFlipMode(void *,int);void ManagedSprite_SetPositionXY(void *,s16,s16);void BattleAnimSystem_CreateTaskDefaultPriority(void *,void (*)(void),void *);void ov07_0222EF88(void);
extern const s16 FX_SinCosTable_[];
void ov07_0222F0B0(void *,void *,void *,void *);
void ov07_0222F0B0(void *ctx,void *system,void *manager,void *first) {
 State *s=BattleAnimUtil_AllocWork(ctx,144);s16 x,y,dx,dy;Template t;int i;
 s->ctx=ctx;s->system=system;s->manager=manager;
 s->pic=BattleAnimSystem_GetBattlerPokepic(s->ctx,BattleAnimSystem_GetAttacker(s->ctx));
 x=Pokepic_GetAttr(s->pic,0);y=Pokepic_GetAttr(s->pic,1);s->y=y;
 s->attr=ov07_0221FAA0(s->ctx,BattleAnimSystem_GetAttacker(s->ctx));BattleAnimSystem_GetSpriteTemplate(&t,s->ctx);
 for(i=0;i<6;i++) {
  int angle;
  if(i==0)s->sprites[i]=first;else s->sprites[i]=SpriteSystem_NewSprite(system,manager,&t);
  ManagedSprite_SetDrawFlag(s->sprites[i],0);
  angle=((i/2)*0x1555)>>4;
  dx=(s32)(((s64)FX_SinCosTable_[angle*2+1]*0x30000+0x800)>>12)>>12;
  dy=(s32)(((s64)FX_SinCosTable_[angle*2]*0x30000+0x800)>>12)>>12;

  if(i%2==0){dx=x+dx;ManagedSprite_SetFlipMode(s->sprites[i],1);}else dx=x-dx;
  dy=y-dy;ManagedSprite_SetPositionXY(s->sprites[i],dx,dy);s->priority[i]=i<2?8:0;
 }
 BattleAnimSystem_CreateTaskDefaultPriority(s->ctx,ov07_0222EF88,s);
}

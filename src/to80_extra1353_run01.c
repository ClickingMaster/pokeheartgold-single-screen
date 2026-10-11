#pragma force_active on
#include "global.h"
typedef struct Work { void *ctx,*other; int pad[2]; void *a; int y; void *sprite0,*sprite1,*sprite2,*sprite3; char gap[0x9c]; void *extra,*last0,*last1; } Work;
extern Work *BattleAnimUtil_AllocWork(void*,int);
extern void *BattleAnimSystem_GetBattlerSpriteManager(void*),*BattleAnimSystem_GetPaletteData(void*),*BattleAnimSystem_GetBattlerPokepic(void*,int),*ov07_0221FAA0(void*,int),*BattleAnimSystem_GetBattlerSprite(void*,int),*ov07_0221FB78(void*,int);
extern int BattleAnimSystem_GetAttacker(void*),BattleAnimSystem_GetDefender(void*),Pokepic_GetAttr(void*,int);
extern void ManagedSprite_SetDrawPriority(void*,int),ManagedSprite_SetPriority(void*,int),ManagedSprite_SetPositionXY(void*,s16,s16),ManagedSprite_SetDrawFlag(void*,int),ManagedSprite_SetOamMode(void*,int),ManagedSprite_SetAffineOverwriteMode(void*,int),ManagedSprite_SetAffineTranslation(void*,int,int),ov07_0222F764(void),BattleAnimSystem_CreateTaskDefaultPriority(void*,void(*)(void),Work*);
void ov07_0222F848(void*);
void ov07_0222F848(void *ctx) {
 Work *w=BattleAnimUtil_AllocWork(ctx,0xd0); void *pic,*pic2; s16 x,y,x2,y2;
 w->ctx=ctx; w->other=BattleAnimSystem_GetBattlerSpriteManager(ctx); w->extra=BattleAnimSystem_GetPaletteData(w->ctx);
 pic=BattleAnimSystem_GetBattlerPokepic(w->ctx,BattleAnimSystem_GetAttacker(ctx)); x=Pokepic_GetAttr(pic,0); y=Pokepic_GetAttr(pic,1); y-=Pokepic_GetAttr(pic,0x29);
 pic2=BattleAnimSystem_GetBattlerPokepic(w->ctx,BattleAnimSystem_GetDefender(ctx)); x2=Pokepic_GetAttr(pic2,0); y2=Pokepic_GetAttr(pic2,1); y2-=Pokepic_GetAttr(pic2,0x29); w->y=y;
 w->a=ov07_0221FAA0(w->ctx,BattleAnimSystem_GetAttacker(w->ctx));
 w->sprite0=BattleAnimSystem_GetBattlerSprite(w->ctx,0); ManagedSprite_SetDrawPriority(w->sprite0,100); ManagedSprite_SetPriority(w->sprite0,1); ManagedSprite_SetPositionXY(w->sprite0,x,y); ManagedSprite_SetDrawFlag(w->sprite0,0); ManagedSprite_SetOamMode(w->sprite0,1); ManagedSprite_SetAffineOverwriteMode(w->sprite0,2);
 w->sprite1=BattleAnimSystem_GetBattlerSprite(w->ctx,1); ManagedSprite_SetDrawPriority(w->sprite1,100); ManagedSprite_SetPriority(w->sprite1,1); ManagedSprite_SetPositionXY(w->sprite1,x2,y2); ManagedSprite_SetDrawFlag(w->sprite1,0); ManagedSprite_SetOamMode(w->sprite1,1); ManagedSprite_SetAffineOverwriteMode(w->sprite1,2); ManagedSprite_SetAffineTranslation(w->sprite1,0,40);
 w->last0=ov07_0221FB78(w->ctx,0); w->last1=ov07_0221FB78(w->ctx,1); w->sprite2=BattleAnimSystem_GetBattlerSprite(w->ctx,2); w->sprite3=BattleAnimSystem_GetBattlerSprite(w->ctx,3); ManagedSprite_SetDrawFlag(w->sprite2,0); ManagedSprite_SetDrawFlag(w->sprite3,0); BattleAnimSystem_CreateTaskDefaultPriority(w->ctx,ov07_0222F764,w);
}

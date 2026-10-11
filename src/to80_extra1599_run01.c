#pragma force_active on
#include "global.h"
typedef struct {s16 x,y;u8 gap[32];} Movement;
typedef struct {void*ctx,*p4,*sys;int state;u32 p10;void*pics[4];void*sprite;Movement move;s16 x,y;int phase,timer,draw;} S;
extern void ov07_0222E07C(S*),BattleAnimUtil_SetSpritePosWithOffset(Movement*,void*,s16,s16),BattleAnimSystem_DestroyTask(void*,void*);
extern int BattleAnimUtil_UpdateLinearMotion(Movement*),BattleAnimSystem_GetBattleBgPaletteMask(void*);
extern void *BattleAnimSystem_GetPaletteData(void*);
extern void ManagedSprite_SetAnim(void*,int),ManagedSprite_SetAnimateFlag(void*,int),ManagedSprite_SetDrawFlag(void*,int),Sprite_DeleteAndFreeResources(void*),SpriteSystem_DrawSprites(void*),Heap_Free(void*),PaletteData_BeginPaletteFade(void*,int,u16,int,int,int,int),Pokepic_StartPaletteFade(void*,int,int,int,int);
extern int PaletteData_GetSelectedBuffersBitmask(void*);
void ov07_0222E108(void*,S*);
void ov07_0222E108(void *task,S*s){int i;void *pal;
 switch(s->state){
 case 0:if(++s->timer<=1)break;ov07_0222E07C(s);s->state++;s->timer=0;break;
 case 1:if(BattleAnimUtil_UpdateLinearMotion(&s->move)){BattleAnimUtil_SetSpritePosWithOffset(&s->move,s->sprite,s->x,s->y);break;}if(++s->phase<4){s->state=0;s->x+=s->move.x;s->y+=s->move.y;}else{s->state++;s->timer=8;ManagedSprite_SetAnim(s->sprite,1);ManagedSprite_SetAnimateFlag(s->sprite,1);}break;
 case 2:if(--s->timer>=0)break;s->state++;pal=BattleAnimSystem_GetPaletteData(s->ctx);PaletteData_BeginPaletteFade(pal,1,(u16)BattleAnimSystem_GetBattleBgPaletteMask(s->ctx),-8,0,16,65535);for(i=0;i<4;i++)if(s->pics[i])Pokepic_StartPaletteFade(s->pics[i],14,16,0,65535);break;
 case 3:if(PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(s->ctx)))break;s->state++;pal=BattleAnimSystem_GetPaletteData(s->ctx);PaletteData_BeginPaletteFade(pal,1,(u16)BattleAnimSystem_GetBattleBgPaletteMask(s->ctx),-8,16,0,65535);for(i=0;i<4;i++)if(s->pics[i])Pokepic_StartPaletteFade(s->pics[i],2,0,0,65535);break;
 case 4:if(PaletteData_GetSelectedBuffersBitmask(BattleAnimSystem_GetPaletteData(s->ctx)))break;s->state++;s->timer=4;s->phase=4;s->draw=0;break;
 case 5:if(--s->timer>=0)break;s->timer=4;if(--s->phase<0){s->state++;break;}ManagedSprite_SetDrawFlag(s->sprite,s->draw);s->draw^=1;break;
 case 6:Sprite_DeleteAndFreeResources(s->sprite);BattleAnimSystem_DestroyTask(s->ctx,task);Heap_Free(s);return;
 }
 SpriteSystem_DrawSprites(s->sys);
}

#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *owner,*sprites;int state,timer;u8 pad[0x9c-16];u8 fade[20];} Work;
extern void BattleAnimUtil_SetBlendWithBattleBg(void *,int,int),BattleAnimUtil_StartBlendAlphaMotion(void *,int,int,int,int,int),ov07_0222F434(Work *),ov07_0222F600(Work *),BattleAnimSystem_DestroyTask(void *,void *),Heap_Free(void *),SpriteSystem_DrawSprites(void *);
extern int BattleAnimUtil_IsBlendAlphaMotionDone(void *),ov07_0222F4D4(Work *),ov07_0222F6C0(Work *);
void ov07_0222F764(void *,Work *);
void ov07_0222F764(void *task,Work *w){switch(w->state){case 0:BattleAnimUtil_SetBlendWithBattleBg(w->owner,0,16);BattleAnimUtil_StartBlendAlphaMotion(w->fade,0,8,16,8,8);w->state++;break;case 1:if(BattleAnimUtil_IsBlendAlphaMotionDone(w->fade)){w->state++;ov07_0222F434(w);}break;case 2:if(ov07_0222F4D4(w)){w->state++;w->timer=2;}break;case 3:if(--w->timer<0){ov07_0222F600(w);w->state++;}break;case 4:if(ov07_0222F6C0(w)){w->state++;BattleAnimUtil_StartBlendAlphaMotion(w->fade,8,0,8,16,8);}break;case 5:if(BattleAnimUtil_IsBlendAlphaMotionDone(w->fade))w->state++;break;case 6:BattleAnimSystem_DestroyTask(w->owner,task);Heap_Free(w);break;}SpriteSystem_DrawSprites(w->sprites);}

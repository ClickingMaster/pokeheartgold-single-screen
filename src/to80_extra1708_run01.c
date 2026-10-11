#pragma force_active on
#include "global.h"
#include "heap.h"
#pragma force_active on
typedef struct {u8 step,pad;s16 angle;int value;void*owner,*pic;u8 motion[20];int modifier;} Work;
extern int BattleAnimUtil_UpdateScaleMotionXY(void*),BattleAnimUtil_CalcScaleYOffset(int,int,int);
extern void BattleAnimUtil_InitScaleMotionXY(void*,int,int,int,int,int,int),Pokepic_SetAttr(void*,int,int),BattleAnimSystem_DestroyTask(void*,void*);
void ov07_022278B0(void*,Work*);
#define UPDATE() do {Pokepic_SetAttr(w->pic,12,*(s16*)&w->motion[0]);Pokepic_SetAttr(w->pic,13,*(s16*)&w->motion[2]);{int v=BattleAnimUtil_CalcScaleYOffset(w->angle,w->value,w->modifier);Pokepic_SetAttr(w->pic,1,w->angle+v);}}while(0)
void ov07_022278B0(void*task,Work*w){switch(w->step){case 0:if(BattleAnimUtil_UpdateScaleMotionXY(w->motion)==1){UPDATE();return;}BattleAnimUtil_InitScaleMotionXY(w->motion,150,50,50,150,100,10);w->step++;return;case 1:if(BattleAnimUtil_UpdateScaleMotionXY(w->motion)==1){UPDATE();return;}BattleAnimUtil_InitScaleMotionXY(w->motion,50,100,150,100,100,5);w->step++;return;case 2:if(BattleAnimUtil_UpdateScaleMotionXY(w->motion)==1){UPDATE();return;}BattleAnimUtil_InitScaleMotionXY(w->motion,100,150,100,150,100,5);w->step++;return;case 3:if(BattleAnimUtil_UpdateScaleMotionXY(w->motion)==1){UPDATE();return;}BattleAnimUtil_InitScaleMotionXY(w->motion,150,100,150,100,100,5);w->step++;return;case 4:if(BattleAnimUtil_UpdateScaleMotionXY(w->motion)==1){UPDATE();return;}Pokepic_SetAttr(w->pic,12,256);Pokepic_SetAttr(w->pic,13,256);Pokepic_SetAttr(w->pic,1,w->angle);w->step++;return;default:BattleAnimSystem_DestroyTask(w->owner,task);Heap_Free(w);return;}}

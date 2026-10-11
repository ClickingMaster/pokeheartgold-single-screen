#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov07_022366F5[],ov07_022366F6[],ov07_022366F7[],ov07_022366F8[],ov07_022366F9[],ov07_022366D8[],ov07_022366D9[],ov07_022366DA[];
extern void BattleAnimUtil_InitScaleMotionXY(void*,int,int,int,int,int,int),BattleAnimUtil_InitLinearMotion(void*,int,int,s16,s16,int),Pokepic_SetAttr(void*,int,int),BattleAnimUtil_AlignPokepicToScale(void*,int,int,int,int),BattleAnimSystem_DestroyTask(void*,void*),Heap_Free(void*);extern int BattleAnimUtil_UpdateScaleMotionXY(void*),BattleAnimUtil_UpdateLinearMotion(void*);
void ov07_02228510(void*,u8*);
#pragma opt_common_subs off
void ov07_02228510(void*task,u8*w){switch(w[0]){case 0:{int off=w[1]*5;BattleAnimUtil_InitScaleMotionXY(w+16,ov07_022366F5[off],ov07_022366F6[off],ov07_022366F7[off],ov07_022366F8[off],100,ov07_022366F9[off]);off=w[1]*3;{int x=*(s16*)(w+4);BattleAnimUtil_InitLinearMotion(w+52,0,0,(s16)(x+ov07_022366D8[off]),(s16)(x+ov07_022366D9[off]),ov07_022366DA[off]);}w[1]++;w[0]++;break;}case 1:{int done=0;if(!BattleAnimUtil_UpdateScaleMotionXY(w+16))done++;if(!BattleAnimUtil_UpdateLinearMotion(w+52))done++;if(done>=2){if(w[1]>=3){w[2]++;w[1]=0;if(w[2]>=3)w[0]++;}else w[0]=0;}Pokepic_SetAttr(*(void**)(w+12),12,*(s16*)(w+16));Pokepic_SetAttr(*(void**)(w+12),13,*(s16*)(w+18));BattleAnimUtil_AlignPokepicToScale(*(void**)(w+12),*(s16*)(w+54),*(s16*)(w+6),*(int*)(w+36),0);break;}default:BattleAnimSystem_DestroyTask(*(void**)(w+8),task);Heap_Free(w);break;}}

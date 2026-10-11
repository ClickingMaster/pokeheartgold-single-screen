#pragma force_active on
#include "global.h"
#pragma force_active on
void Pokepic_SetAttr(void *,int,int);
int BattleAnimUtil_UpdateValueMotionFx(void *),BattleAnimUtil_UpdateLinearMotionPokepic(void *,void *);
void BattleAnimUtil_InitValueMotionFx(void *,s16,s16,int),BattleAnimUtil_InitLinearMotion(void *,s16,s16,s16,s16,int);
int ov07_0222E3E8(u8 *);
#define W(o) (*(int *)(w+(o)))
#define P(o) (*(void **)(w+(o)))
#define H(o) (*(s16 *)(w+(o)))
int ov07_0222E3E8(u8 *w){int result=0;switch(W(76)){
case 0:BattleAnimUtil_UpdateValueMotionFx(w+16);Pokepic_SetAttr(P(8),9,W(16));if(--W(80)<0){W(76)++;BattleAnimUtil_InitLinearMotion(w+36,H(12),H(12),H(14),(s16)(H(14)+2),2);}break;
case 1:if(BattleAnimUtil_UpdateValueMotionFx(w+16))Pokepic_SetAttr(P(8),9,W(16));if(!BattleAnimUtil_UpdateLinearMotionPokepic(w+36,P(8))){W(76)++;BattleAnimUtil_InitValueMotionFx(w+16,(s16)W(16),0,3);BattleAnimUtil_InitLinearMotion(w+36,H(12),H(12),(s16)(H(14)+2),H(14),2);}break;
case 2:BattleAnimUtil_UpdateLinearMotionPokepic(w+36,P(8));if(BattleAnimUtil_UpdateValueMotionFx(w+16))Pokepic_SetAttr(P(8),9,W(16));else{Pokepic_SetAttr(P(8),0,H(12));Pokepic_SetAttr(P(8),1,H(14));Pokepic_SetAttr(P(8),9,result);W(76)++;}break;
case 3:result=1;break;}return result;}

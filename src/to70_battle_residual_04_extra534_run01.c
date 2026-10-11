#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x194];u8 battler,kind,flag;} Work;
extern void*NARC_New(int,int),*BattleSystem_GetBattleInput(void*),*BattleSystem_GetHpBar(void*,int);extern void NARC_Delete(void*),BattleInput_ChangeMenu(void*,void*,void*,int,int,int),BattleInput_Deadstriped_022698AC(void*,int),ov12_02265D74(void*),ov12_02264EB4(void*),BattleInput_DisableBallGauge(void*),ov12_02262014(Work*),BattleController_EmitCommandComplete(void*,int,int),ov12_02259928(Work*);extern int BattleSystem_GetBattlerIdPartner(void*,int);
void ov12_02259514(void*,Work*);
void ov12_02259514(void*b,Work*w){void*a,*c,*input;int partner;if(w->flag==0){a=NARC_New(7,5);c=NARC_New(8,5);input=BattleSystem_GetBattleInput(b);BattleInput_ChangeMenu(a,c,input,0,0,0);BattleInput_Deadstriped_022698AC(input,0);NARC_Delete(a);NARC_Delete(c);partner=BattleSystem_GetBattlerIdPartner(b,w->battler);if(partner!=w->battler)ov12_02265D74(BattleSystem_GetHpBar(b,partner));ov12_02264EB4((u8*)w+0x28);BattleInput_DisableBallGauge(input);ov12_02262014(w);}BattleController_EmitCommandComplete(b,w->battler,47);ov12_02259928(w);}

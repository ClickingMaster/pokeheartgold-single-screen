#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "pokemon.h"
#include "party.h"
#pragma force_active on
struct SelectTask {BattleSystem *battle;u32 pad4;u8 state,battler,padA[2],chosen[4],pad10[6],other;};
extern int ov12_0223BE0C(BattleSystem *,int,u8 *);
extern void ov12_02261ED4(BattleSystem *);
extern void ov12_02261EB8(BattleSystem *);
extern void ov12_02263360(BattleSystem *,int,int);
extern void ov12_0226430C(BattleSystem *,int,int);
extern void Heap_Free(void *);

void ov12_0225F980(SysTask *,struct SelectTask *);
void ov12_0225F980(SysTask *task,struct SelectTask *state) {u8 selection; if(ov12_0223BE0C(state->battle,state->battler,&selection)==1)ov12_02261ED4(state->battle); if(selection==0 || selection>6) ov12_02261EB8(state->battle); else { int slot=selection-1; Party *party=BattleSystem_GetParty(state->battle,state->battler); if(slot==state->chosen[state->battler]||slot==state->other)ov12_02261EB8(state->battle);else {int count=selection; if(count>Party_GetCount(party))ov12_02261EB8(state->battle);else { Pokemon *mon=BattleSystem_GetPartyMon(state->battle,state->battler,count-1); if(GetMonData(mon,163,NULL)==0)ov12_02261EB8(state->battle); if(GetMonData(mon,174,NULL)==494)ov12_02261EB8(state->battle); } } } ov12_02263360(state->battle,state->battler,selection);ov12_0226430C(state->battle,state->battler,state->state);Heap_Free(state);SysTask_Destroy(task); }

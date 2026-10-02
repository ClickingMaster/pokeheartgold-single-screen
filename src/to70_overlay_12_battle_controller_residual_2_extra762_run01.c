#pragma force_active on
#include "global.h"
extern void ov12_02262240(void *,int,int,void *,int);
extern int BattleSystem_GetBattlerIdPartner(void *,int);
typedef struct {u8 command,index;u16 value;} Packet;
void BattleController_EmitTrainerEncounter(void *system,int id);
void BattleController_EmitThrowPokeball(void *system,int id,u8 arg);
void BattleController_EmitTrainerEncounter(void *system,int id){Packet p;p.command=8;p.value=*(u8*)((u32)system+id*52+0xad);p.index=*(u8*)((u32)system+id+0xa8);ov12_02262240(system,1,id,&p,4);}
void BattleController_EmitThrowPokeball(void *system,int id,u8 arg){Packet p;p.command=9;p.index=arg;{int partner=BattleSystem_GetBattlerIdPartner(system,id);u8 *base=*(void**)((u8*)system+48);p.value=*(u8*)((u32)base+partner+0x219c);}ov12_02262240(system,1,id,&p,4);}

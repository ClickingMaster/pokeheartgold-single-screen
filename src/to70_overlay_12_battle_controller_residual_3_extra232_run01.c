#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Record{u8 pad;u8 role;u8 tail[50];}Record;typedef struct System{u8 pad[0xa8];u8 ids[4];Record records[4];}System;typedef struct Message{u8 command,id;u16 role;u32 value;}Message;
extern void ov12_02262240(System*,int,int,void*,int);void BattleController_EmitTrainerSlideIn(System*,int,u32);
void BattleController_EmitTrainerSlideIn(System*system,int battler,u32 value){Message msg;msg.command=11;msg.role=system->records[battler].role;msg.id=system->ids[battler];msg.value=value;ov12_02262240(system,1,battler,&msg,8);}

typedef struct Mon{u16 species;u8 pad2[0x12];u32 flags;u8 pad18[0x1c];u8 level;u8 pad35[0x17];u32 hp,maxhp;u8 pad54[0x10];u32 exp;u8 pad68[0x16];u8 form:4;u8 flags2:4;u8 tail[0x41];}Mon;
typedef struct Context{u8 pad[0x219c];u8 selected[4];u8 pad21a0[0xba0];Mon mons[4];}Context;
typedef struct HealthMessage{u8 command,level;u16 hp,maxhp;u8 slot;u8 status:5;u8 form:2;u8 caught:1;u32 exp,maxexp,safari;u8 flag;u8 pad[3];}HealthMessage;
extern void*BattleSystem_GetPartyMon(System*,int,u8);extern u32 GetMonData(void*,int,void*),GetMonExpBySpeciesAndLevel(int,int);extern int GetBattlerStatusCondition(Context*,int),BattleSystem_CheckMonCaught(System*,u16),BattleSystem_GetSafariBallCount(System*);void BattleController_EmitHealthbarSlideIn(System*,Context*,int,int);
void BattleController_EmitHealthbarSlideIn(System*system,Context*context,int battler,int flag){HealthMessage msg;void*mon=BattleSystem_GetPartyMon(system,battler,context->selected[battler]);int species=GetMonData(mon,5,0);int level=GetMonData(mon,161,0);msg.command=12;msg.level=context->mons[battler].level;msg.hp=context->mons[battler].hp;msg.maxhp=context->mons[battler].maxhp;msg.slot=context->selected[battler];msg.status=(u8)GetBattlerStatusCondition(context,battler);if((context->mons[battler].species==29||context->mons[battler].species==32)&&!(context->mons[battler].flags>>31))msg.form=2;else msg.form=context->mons[battler].form;msg.exp=context->mons[battler].exp-GetMonExpBySpeciesAndLevel(species,level);int next=GetMonExpBySpeciesAndLevel(species,level+1);msg.maxexp=next-GetMonExpBySpeciesAndLevel(species,level);msg.caught=(u8)BattleSystem_CheckMonCaught(system,context->mons[battler].species);msg.safari=BattleSystem_GetSafariBallCount(system);msg.flag=flag;ov12_02262240(system,1,battler,&msg,24);}

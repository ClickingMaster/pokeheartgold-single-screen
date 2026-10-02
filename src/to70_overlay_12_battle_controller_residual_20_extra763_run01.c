#pragma force_active on
#include "global.h"
extern u32 BattleSystem_GetBattleType(void *);
extern u32 BattleSystem_GetBattleOutcomeFlags(void *);
extern int sub_0202FC48(void);
extern u16 ov12_0223BE68(void *,u8 *);
extern void ov12_02262240(void *,int,int,void *,int);
typedef struct {u8 command,pad;volatile u16 count;u32 outcome;u8 data[28];} Packet;
void BattleController_EmitSetBattleResults(void *system);
void BattleController_EmitSetBattleResults(void *system){u32 type=BattleSystem_GetBattleType(system);Packet p;p.command=65;p.outcome=BattleSystem_GetBattleOutcomeFlags(system);p.count=0;if((type&4)&&sub_0202FC48()==1&&!(*(u32*)((u8*)system+0x240c)&16)){p.count=ov12_0223BE68(system,p.data);if(p.count>28)GF_AssertFail();}ov12_02262240(system,1,0,&p,36);}

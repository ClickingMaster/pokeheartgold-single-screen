#pragma force_active on
#include "global.h"
#include "battle/battle_controller.h"
#include "battle/battle_system.h"
#include "unk_0202FBCC.h"
#pragma force_active on
typedef struct {u8 command,reserved;u16 length;u8 data[28];} Packet;
void ov12_02262240(BattleSystem*,int,int,const void*,u8);
void BattleController_EmitShowWaitMessage(BattleSystem *system,int battler){Packet p;u32 type=BattleSystem_GetBattleType(system);p.command=55;p.length=0;if((type&4)&&sub_0202FC48()==1&&!(system->battleSpecial&16)){p.length=ov12_0223BE68(system,p.data);if(p.length>=28)GF_AssertFail();ov12_02262240(system,1,battler,&p,32);}}

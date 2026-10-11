#ifndef TO56_R2_W18_PRIVATE_H
#define TO56_R2_W18_PRIVATE_H
#include "global.h"

#include "battle/battle_system.h"

#include "party.h"
#include "pokemon.h"
#include "sys_task_api.h"
int ov12_02258BA0(BattleSystem *, int);
int ov12_02258800(BattleSystem *, int);
void BattleController_ReplyPartySelection(BattleSystem *, int, u32);
void BattleController_EmitCommandComplete(BattleSystem *, int, int);
void ov12_0225F8AC(SysTask *, void *);
#endif

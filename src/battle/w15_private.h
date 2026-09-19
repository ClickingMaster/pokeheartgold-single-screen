#ifndef TO53_SOL_R1_W15_PRIVATE_H
#define TO53_SOL_R1_W15_PRIVATE_H

#include "global.h"
#include "battle/battle.h"
#include "battle/battle_hp_bar.h"
#include "battle/battle_system.h"
#include "heap.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task_api.h"

struct PokepicMoveTask;
struct ManagedSpriteDeleteTask;
struct ManagedSpriteMoveTask;

void ov12_0226430C(BattleSystem *battleSystem, int battlerId, int command);
void ov12_0225CDB8(SysTask *task, struct PokepicMoveTask *work);
void ov12_0225D890(SysTask *task, struct ManagedSpriteDeleteTask *work);
void ov12_0225D990(SysTask *task, struct ManagedSpriteMoveTask *work);
void ov12_0225DA18(SysTask *task, BattleHpBar *hpBar);
void ov12_0225DA8C(SysTask *task, BattleHpBar *hpBar);

#endif

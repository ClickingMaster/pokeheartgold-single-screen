#ifndef TO53_SOL_R1_W16_PRIVATE_H
#define TO53_SOL_R1_W16_PRIVATE_H

#include "global.h"
#include "battle/battle_system.h"
#include "battle/overlay_12_0224E4FC.h"
#include "pokemon.h"

void BattleController_EmitCommandComplete(BattleSystem *, int, int);
void BattleController_ReplyCommandSelection(BattleSystem *, int, u32);
void ov12_02261EB8(BattleSystem *);
void ov12_02261ED4(BattleSystem *);
u32 ov10_022205BC(BattleSystem *, int);
void ov12_0225E104(SysTask *, void *);
void ov12_0225E134(SysTask *, void *);
void ov12_0225E154(SysTask *, void *);
void ov12_0225E1D4(SysTask *, void *);
void ov12_0225E1FC(SysTask *, void *);
void ov12_0225E4CC(SysTask *, void *);
void ov12_0225E4EC(SysTask *, void *);
void ov12_0225E6FC(SysTask *, void *);
void ov12_0225E740(SysTask *, void *);
void ov12_0225F3A4(SysTask *, void *);
void ov12_0225F3FC(SysTask *, void *);
void ov12_0225F434(SysTask *, void *);

#endif

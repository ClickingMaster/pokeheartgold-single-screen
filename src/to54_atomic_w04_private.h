#ifndef TO54_W04_PRIVATE_H
#define TO54_W04_PRIVATE_H

#include "global.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "pokepic.h"
#include "sys_task_api.h"

void ov12_0225D890(SysTask *, void *);
void ov12_0225DA8C(SysTask *, void *);
void ov12_022605D0(SysTask *, void *);

void ov12_0225A334(BattleSystem *, OpponentData *, void *);
void ov12_0225A4DC(BattleSystem *, OpponentData *, void *);
void ov12_0225A818(BattleSystem *, OpponentData *, u8 *);
void ov12_0225A8C4(BattleSystem *, OpponentData *, BattleMessage *);
void ov12_0225A018(BattleSystem *, OpponentData *, u8 *);
void ov12_0225A604(BattleSystem *, OpponentData *, u8 *);
void ov12_0225A7AC(BattleSystem *, OpponentData *, u8 *);
void ov12_0225CDB8(SysTask *, void *);
extern const u16 ov12_0226D15A[];
void ov12_02264FB0(void *, int);

#endif

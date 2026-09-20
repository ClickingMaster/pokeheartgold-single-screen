#ifndef TO56_R4_W02_PRIVATE_H
#define TO56_R4_W02_PRIVATE_H
#include "global.h"

#include "battle/battle.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "render_window.h"
#include "sys_task.h"
typedef struct W02TaskData {
    void *battleSystem;
    u8 command;
    u8 battlerId;
    u8 value195;
    u8 unk07;
    u8 payload[6];
    u8 mode;
    u8 pad0F;
} W02TaskData;
void ov12_02260BA0(SysTask *, void *);
void ov12_02260C58(SysTask *, void *);
void ov12_0225B0A0(void *, u8 *, u8 *);
void ov12_0225B0E8(void *, u8 *, u8 *);
void ov12_0225B120(void *, u8 *, u8 *);
void ov12_0225B16C(void *, u8 *, u8 *);
SysTask *SysTask_CreateOnMainQueue(SysTaskFunc, void *, u32);
void ov12_0225B1A8(BattleSystem *, OpponentData *);
void ov12_0223BB80(BattleSystem *, void *);
void ov12_0226430C(BattleSystem *, int, int);
#endif

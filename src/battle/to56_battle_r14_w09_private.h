#ifndef TO56_W09_PRIVATE_H
#define TO56_W09_PRIVATE_H
#include "global.h"

#include "battle/battle.h"
#include "battle/battle_system.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07.h"
#include "sound.h"
#include "sprite_system.h"
#include "sys_task_api.h"

typedef struct TrainerTerrain {
    ManagedSprite *managedSprite;
} TrainerTerrain;

typedef struct TrainerEncounterData {
    BattleSystem *battleSystem;
    void *pokepic;
    TrainerTerrain *terrain;
    ManagedSprite *managedSprite;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 face;
    s16 targetX;
    u16 ballFlashStarted : 1;
    u16 padding_16_1 : 15;
    int battlerType;
    int delay;
    int enterFrameCount;
} TrainerEncounterData;

typedef struct TrainerThrowBallData {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 face;
    int backSpriteIdx;
    int ballTypeIn;
    int delay;
    int ballTargetState;
} TrainerThrowBallData;

extern const s16 ov12_0226D1E8[][6][2];
extern const s16 ov12_0226D1EA[][6][2];
void ov12_0225CE28(SysTask *, TrainerEncounterData *);
void ov12_0225D138(SysTask *, TrainerThrowBallData *);
void ov12_0225D644(SysTask *, TrainerThrowBallData *);
void PlaySE(u16);
void BattleController_EmitCommandComplete(BattleSystem *, int, int);
BOOL ov12_02261DC8(TrainerEncounterData *, ManagedSprite *);
void ov07_0223449C(UnkBallData *, int);
void ov07_022344A8(UnkBallData *, s16, s16);
void ov07_022344C0(UnkBallData *, u32);
void ov07_022344C4(UnkBallData *, u16);
void ov07_022344D0(UnkBallData *, int);
void ov07_022344DC(UnkBallData *, s32);
void ov07_02233EFC(UnkBallData *, u32);
BOOL ov07_02233E88(UnkBallData *);

#endif

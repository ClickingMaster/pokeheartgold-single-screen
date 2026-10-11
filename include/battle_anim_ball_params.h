#ifndef POKEHEARTGOLD_BATTLE_ANIM_BALL_PARAMS_H
#define POKEHEARTGOLD_BATTLE_ANIM_BALL_PARAMS_H

// The ball throw API (src/battle/battle_anim_ball.c) as overlay 12's send-out tasks in
// src/to91_battle41410_combined_run01.c see it. The real types are UnkStruct_134 / UnkBallData
// (battle/battle.h, overlay_07.h); delete this header once that file uses them.

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "math_util.h"
#include "particle_system.h"
#include "pokepic.h"
#include "seal_case.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_02091054.h"

// = UnkStruct_134
typedef struct Ov07Params {
    u32 kind;
    enum HeapID heapId;
    u32 callbackId;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    SpriteSystem *spriteSystem;
    u32 unk20;
    u32 unk24;
} Ov07Params;

// = UnkBallData
typedef struct Ov07CallbackWork Ov07CallbackWork;

extern const s16 ov07_022377DC[][2];
extern const s16 ov07_022377F4[][3];

Ov07CallbackWork *UnkBallData_New(Ov07Params *params);
BOOL UnkBallData_IsActive(Ov07CallbackWork *work);
BOOL UnkBallData_HasOpened(Ov07CallbackWork *work);
void UnkBallData_Delete(Ov07CallbackWork *work);
u32 UnkBallData_GetMode(Ov07CallbackWork *work);

#endif // POKEHEARTGOLD_BATTLE_ANIM_BALL_PARAMS_H

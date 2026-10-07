#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "bg_window.h"
#include "heap.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sys_task_api.h"

typedef struct TrainerEncounterMessage {
    u8 unk0;
    u8 unk1;
    u16 trainerClass;
    int unk4;
} TrainerEncounterMessage;

typedef struct TrainerEncounterTask {
    BattleSystem *bsys;
    Pokepic *pokepic;
    UnkBattleSystemSub17C *unk8;
    ManagedSprite *sprite;
    u8 unk10;
    u8 battlerId;
    u8 state;
    u8 unk13;
    s16 unk14;
    u8 pad16[2];
    int battlerType;
    int unk1C;
    int unk20;
} TrainerEncounterTask;

typedef struct BattlePos {
    s16 x;
    s16 y;
    s16 z;
} BattlePos;

extern const BattlePos ov07_022377F4[];
extern const s16 ov07_022377DC[][2];
u8 ov12_02261EF0(BattleSystem *bsys, s32 battlerId, u8 trainerClass);
ManagedSprite *ov12_02261B2C(BattleSystem *bsys, s32 arg1, int trainerClass, int resTag, BOOL isLink, s16 x, s16 y);
void ManagedSprite_SetPositionXY(ManagedSprite *sprite, s16 x, s16 y);
void ov12_0225CE28(SysTask *task, void *data);
void ov12_0225A07C(BattleSystem *bsys, OpponentData *opponentData, TrainerEncounterMessage *msg);
// @fn ov12_0225A07C
#pragma opt_common_subs off
void ov12_0225A07C(BattleSystem *bsys, OpponentData *opponentData, TrainerEncounterMessage *msg) {
    PokepicManager *pokepicManager;
    u32 battleType;
    BOOL isLink;
    s16 y;
    s16 x;
    int idx;
    PokepicTemplate template;
    UnkStruct_02070D3C files;
    TrainerEncounterTask *task = Heap_Alloc(HEAP_ID_BATTLE, sizeof(TrainerEncounterTask));

    task->state = 0;
    if (opponentData->battlerType & 1) {
        task->unk13 = 2;
        task->unk8 = ov12_0223A8F4(bsys, 1);
        ManagedSprite_SetPositionXY(task->unk8->unk0, ov07_022377F4[opponentData->battlerType & 1].x, 88);
    } else {
        task->unk13 = 0;
        task->unk8 = ov12_0223A8F4(bsys, 0);
        ManagedSprite_SetPositionXY(task->unk8->unk0, ov07_022377F4[opponentData->battlerType & 1].x, 136);
    }
    if ((BattleSystem_GetBattleType(bsys) & BATTLE_TYPE_MULTI) || ((BattleSystem_GetBattleType(bsys) & BATTLE_TYPE_TAG) && (opponentData->battlerType & 1))) {
        idx = opponentData->battlerType;
    } else {
        idx = opponentData->battlerType & 1;
    }
    msg->trainerClass = ov12_02261EF0(bsys, opponentData->unk194, msg->trainerClass);
    battleType = BattleSystem_GetBattleType(bsys);
    isLink = FALSE;
    if (ov12_0223C140(bsys, opponentData->unk194) != 0xFF) {
        if ((battleType & BATTLE_TYPE_DOUBLES) && !(battleType & BATTLE_TYPE_MULTI)) {
            isLink = FALSE;
        } else {
            isLink = TRUE;
        }
    }
    x = ov07_022377F4[idx].x;
    y = ov07_022377F4[idx].y;
    opponentData->managedSprite = ov12_02261B2C(bsys, idx, msg->trainerClass, opponentData->battlerType, isLink, x, y);
    task->sprite = opponentData->managedSprite;
    if (task->unk13 == 0 && (BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_NONE || BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_SAFARI || BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_ROAMER || BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_PAL_PARK || BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_TUTORIAL || BattleSystem_GetBattleType(bsys) == BATTLE_TYPE_BUG_CONTEST)) {
        pokepicManager = BattleSystem_GetPokepicManager(bsys);
        sub_02070D84(msg->trainerClass, task->unk13, &files);
        template.narcID = files.narcId;
        template.charDataID = files.ncbr_id;
        template.palDataID = files.nclr_id;
        template.species = 0;
        template.isAnimated = FALSE;
        template.personality = 0;
        task->pokepic = PokepicManager_CreatePokepic(pokepicManager, &template, x, y, ov07_022377F4[idx].z, opponentData->unk194, NULL, NULL);
    } else {
        task->pokepic = NULL;
    }
    task->unk14 = ov07_022377DC[idx][0];
    task->bsys = bsys;
    task->unk10 = msg->unk0;
    task->battlerId = opponentData->unk194;
    task->battlerType = opponentData->battlerType;
    task->unk20 = 0;
    if (task->battlerType == 0 || task->battlerType == 2) {
        BgSetPosTextAndCommit(BattleSystem_GetBgConfig(bsys), 3, BG_POS_OP_SUB_X, 132);
    }
    SysTask_CreateOnMainQueue(ov12_0225CE28, task, 0);
}
#pragma opt_common_subs reset

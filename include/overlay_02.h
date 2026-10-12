#ifndef POKEHEARTGOLD_OVERLAY_02_H
#define POKEHEARTGOLD_OVERLAY_02_H

#include "battle/battle_setup.h"
#include "field/field_3d_object_task.h"
#include "field/headbutt.h"

#include "heap.h"
#include "script.h"
#include "start_menu.h"
#include "sys_task.h"

typedef struct FieldMoveTaskEnvironment FieldMoveTaskEnvironment;

typedef struct SafariDecorationArgs {
    SaveData *saveData;
    BOOL *unk04; // points to fieldSystem->unk_10C
    u8 unk08;
    u8 deltaX;
    u8 unk0A;
    u8 deltaY;
    u8 unk0C;
    u8 unk0D;
    u8 unk0E; // not set
    u8 unk0F;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u16 unk1C; // known from what's decompiled
    u8 *unk20; // points to fieldSystem->unk_111
} SafariDecorationArgs;

SysTask *FlyLanding_Start(FieldSystem *fieldSystem, int gender);
BOOL FlyLanding_IsDone(SysTask *task);
void FlyLanding_Delete(SysTask *task);
struct FieldMoveTaskEnvironment *CreateFieldEscapeRopeTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID);
struct FieldMoveTaskEnvironment *FieldMoveTask_CreateTeleportEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID);
struct FieldMoveTaskEnvironment *FieldMoveTask_CreateDigEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID);
BOOL Task_FieldEscapeRope(TaskManager *taskManager);
BOOL Task_FieldTeleport(TaskManager *taskManager);
BOOL Task_FieldDig(TaskManager *taskManager);
BOOL Task_FieldMoveArrival(TaskManager *taskManager);
FieldMoveTaskEnvironment *FieldMoveTask_CreateArrivalEnvironment(FieldSystem *fieldSystem, enum HeapID heapID, int kind);
Field3dObjectTask *RockDebris_Create(FieldSystem *fieldSystem);
Field3dObjectTask *RockDebris_CreateWithFollower(FieldSystem *fieldSystem);
void RockDebris_Delete(Field3dObjectTask *task);

// TODO: define the struct, figure out what a1 is
void *ov02_02249458(FieldSystem *fieldSystem, int a1, Pokemon *mon, int gender);
BOOL ov02_0224953C(void *work);
void ov02_02249548(void *work);

void ov02_02245B80(TaskManager *taskManager);
void ShowTrapEffect(TaskManager *taskManager, u8 kind);
void ShowLegendaryWing(TaskManager *taskManager, u8 kind);
void ShowHideoutAlarm(TaskManager *taskManager);
void PokecenterAnimCreate(FieldSystem *fieldSystem, u8 kind);
BOOL PokecenterAnimRun(TaskManager *taskManager);
void ElevatorAnimCreate(FieldSystem *fieldSystem, u8 direction, u8 length);
int PrimoPassword_CheckWallpaper(PlayerProfile *profile, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID);
int PrimoPassword_Check2(PlayerProfile *profile, u16 word1, u16 word2, u16 word3, u16 word4, enum HeapID heapID);
void ov02_0224BF58(FieldSystem *fieldSystem, u8 tag);
void ov02_0224BFC0(FieldSystem *fieldSystem, u8 tag);
void ov02_0224BFCC(FieldSystem *fieldSystem, u8 tag);
void HallOfFame_StartBallAnim(FieldSystem *fieldSystem, u8 numBalls);
void ScriptFieldMoveEffect_Start(FieldSystem *fieldSystem, u16 *done, int kind, enum HeapID heapID);
void MapObjects_SwapPositions(LocalMapObject *object1, LocalMapObject *object2, TaskManager *taskManager);
void FieldSystem_FollowMonInteract(FieldSystem *fieldSystem);
BOOL FieldSystem_FirstAliveMonHasType(FieldSystem *fieldSystem, u8 type);
void ov02_022507B4(FieldSystem *fieldSystem, u8 count);
BOOL ov02_022508B4(FieldSystem *fieldSystem);
void ov02_022523B4(TaskManager *taskManager);

void ov02_BattleExit_HandleRoamerAction(FieldSystem *fieldSystem, BattleSetup *setup);
void ShakeScreen(TaskManager *taskManager, u32 amplitudeX, u32 amplitudeY, u32 repeats, u16 period);
void LanceFliesAway(TaskManager *taskManager, LocalMapObject *lance, LocalMapObject *dragoniteObject);
void OpenAlphHiddenRoom(TaskManager *taskManager, u32 msgNo);
int FieldSystem_GetSafariAreaIndex(FieldSystem *fieldSystem);

u16 FieldSystem_CheckSafariObjectPlacement(FieldSystem *fieldSystem);
u16 FieldSystem_GetFacingSafariObject(FieldSystem *fieldSystem, u16 *objectIndex);

SafariDecorationArgs *SafariDecoration_CreateArgs(FieldSystem *fieldSystem, enum HeapID heapID);

#endif // POKEHEARTGOLD_OVERLAY_02_H

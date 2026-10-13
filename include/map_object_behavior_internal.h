#ifndef POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_INTERNAL_H
#define POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_INTERNAL_H

#include "field_system.h"
#include "map_object.h"
#include "player_avatar.h"
#include "sys_task_api.h"

typedef struct TrainerBehaviorState {
    u8 state;
    u8 turnState;
    s8 count;
    s8 limit;
    s8 direction;
    s8 sequence;
    s8 index;
    s8 turns;
    s8 timer;
} TrainerBehaviorState;

typedef struct ApricornTreeState {
    u32 spriteId;
    u32 apricorns;
    void *billboard;
    VecFx32 savedState;
} ApricornTreeState;

typedef struct MapObjectSpriteState {
    u16 unk0;
    u16 unk2;
} MapObjectSpriteState;

typedef struct EngageParams {
    int x;
    int z;
    int scriptId;
    int trainerNum;
    int isDouble;
    LocalMapObject *object;
} EngageParams;

typedef struct TrainerApproachWork {
    int state;
    int done;
    int direction;
    int distance;
    int unk10;
    int encounterType;
    int trainerIndex;
    int timer;
    int effect;
    LocalMapObject *trainer;
    PlayerAvatar *player;
    FieldSystem *fieldSystem;
} TrainerApproachWork;

typedef struct FourValues {
    u32 values[4];
} FourValues;

typedef struct TwoByTwoValues {
    u32 values[2][2];
} TwoByTwoValues;

typedef struct DirectionMovementTable {
    u32 values[2][4];
} DirectionMovementTable;

typedef void (*MapObjectAction)(LocalMapObject *object);
typedef BOOL (*MapObjectPredicate)(LocalMapObject *object);
typedef int (*TrainerSightFunc)(LocalMapObject *object, int range, int targetX, int targetZ, int unused);
typedef BOOL (*TrainerApproachStateFunc)(TrainerApproachWork *work);

extern const FourValues gLookSidewaysAxisByFacing;
extern const TwoByTwoValues gLookSidewaysDirections;
extern const DirectionMovementTable gTypeBehaviorSpinOrders;
extern const MapObjectAction gMapObjectTypeBehaviorInits[];
extern const MapObjectPredicate gMapObjectTypeBehaviorChecks[];
extern const MapObjectPredicate gMapObjectTypeBehaviorUpdates[];
extern const TrainerSightFunc _020FE194[];
extern const TrainerApproachStateFunc gTrainerApproachStateFuncs[];

// Defined elsewhere without a header yet.
extern const u32 ov01_02207274[];
int MapObject_RequestBillboard(LocalMapObject *object, void **billboard, u32 spriteId);
void MapObject_CancelAndFreeBillboard(LocalMapObject *object, void **billboard, u32 spriteId);
BOOL MapObject_IsBillboardAnimPaused(LocalMapObject *object);
void MapObject_SetBillboardVisibleFromFlags(LocalMapObject *object, void *billboard);
void BillboardObject_SaveState(void *billboard, VecFx32 *state);
void BillboardObject_RestoreState(void *billboard, VecFx32 *state);
void ApricornRegrowth_Spawn(LocalMapObject *object);

void MapObject_InitTypeBehavior(LocalMapObject *object);
BOOL MapObject_UpdateTypeBehavior(LocalMapObject *object);
void MapObject_CallTypeBehaviorInit(LocalMapObject *object);
BOOL MapObject_CallTypeBehaviorCheck(LocalMapObject *object);
BOOL MapObject_CallTypeBehaviorUpdate(LocalMapObject *object);
BOOL MapObject_IsOffPreviousTile(LocalMapObject *object);
BOOL MapObject_IsOnPreviousTile(LocalMapObject *object);
void TypeBehavior_NoOpInit(LocalMapObject *object);
int TypeBehavior_NoOpCheck(LocalMapObject *object);
int TypeBehavior_NoOpUpdate(LocalMapObject *object);
void TypeBehavior_LookSidewaysInit(LocalMapObject *object);
BOOL TypeBehavior_LookSidewaysCheck(LocalMapObject *object);
BOOL TypeBehavior_LookSidewaysUpdate(LocalMapObject *object);
void TypeBehavior_SpinInit(LocalMapObject *object);
BOOL TypeBehavior_SpinCheck(LocalMapObject *object);
BOOL TypeBehavior_SpinUpdate(LocalMapObject *object);
void ApricornTreeObject_Init(LocalMapObject *object);
void ApricornTreeObject_Update(LocalMapObject *object);
void ApricornTreeObject_Delete(LocalMapObject *object);
void ApricornTreeObject_Pause(LocalMapObject *object);
void ApricornTreeObject_Resume(LocalMapObject *object);
void *ApricornTreeObject_GetBillboard(LocalMapObject *object);
u32 GetApricornTreeSpriteId(int apricorn, int apricornCount);
BOOL TryGetSeenByNpcTrainers(FieldSystem *fieldSystem, BOOL doublesEligible);
BOOL CheckSeenByNpcTrainers(FieldSystem *fieldSystem, MapObjectManager *manager, PlayerAvatar *avatar, LocalMapObject *exclude, EngageParams *out);
void GetEngagingTrainerParams(EngageParams *out, LocalMapObject *object, int x, int z);
int MapObject_GetTrainerSightType(LocalMapObject *object);
int TrainerSight_FindPlayer(LocalMapObject *object, PlayerAvatar *player, int *outDir);
int TrainerSight_GetDistanceInDirection(LocalMapObject *object, int direction, int range, int targetX, int targetZ, int unused);
int TrainerSight_GetDistanceNorth(LocalMapObject *object, int range, int targetX, int targetZ, int unused);
int TrainerSight_GetDistanceSouth(LocalMapObject *object, int range, int targetX, int targetZ, int unused);
int TrainerSight_GetDistanceWest(LocalMapObject *object, int range, int targetX, int targetZ, int unused);
int TrainerSight_GetDistanceEast(LocalMapObject *object, int range, int targetX, int targetZ, int unused);
int TrainerSight_IsPathBlocked(LocalMapObject *object, int direction, int distance);
u32 MapObject_GetTrainerNum(LocalMapObject *object);
u32 MapObjectBehavior_GetTrainerNum(LocalMapObject *object);
LocalMapObject *MapObjectManager_FindTrainerPartner(void *unused, MapObjectManager *manager, LocalMapObject *exclude, u32 trainerNum);
SysTask *TrainerApproach_Start(FieldSystem *fieldSystem, LocalMapObject *trainer, PlayerAvatar *player, int movement, int remaining, int unk10, int unk14, int unk18);
BOOL TrainerApproach_IsDone(SysTask *task);
void TrainerApproach_Delete(SysTask *task);
SysTask *TrainerApproach_CreateTask(FieldSystem *fieldSystem, LocalMapObject *trainer, PlayerAvatar *player, int movement, int remaining, int unk10, int unk14, int unk18);
BOOL TrainerApproach_GetDone(SysTask *task);
void TrainerApproach_DestroyTask(SysTask *task);
void TrainerApproach_Task(SysTask *task, void *data);
BOOL TrainerApproach_UnpauseTrainer(TrainerApproachWork *work);
BOOL TrainerApproach_FaceDirection(TrainerApproachWork *work);
BOOL TrainerApproach_WaitForPlayer(TrainerApproachWork *work);
BOOL TrainerApproach_StartFace(TrainerApproachWork *work);
BOOL TrainerApproach_WaitFace(TrainerApproachWork *work);
BOOL TrainerApproach_SpawnExclamation(TrainerApproachWork *work);
BOOL TrainerApproach_WaitExclamation(TrainerApproachWork *work);
BOOL TrainerApproach_StartReveal(TrainerApproachWork *work);
BOOL TrainerApproach_WaitReveal(TrainerApproachWork *work);
BOOL TrainerApproach_Wait(TrainerApproachWork *work);
BOOL TrainerApproach_CheckDistance(TrainerApproachWork *work);
BOOL TrainerApproach_StartStep(TrainerApproachWork *work);
BOOL TrainerApproach_WaitStep(TrainerApproachWork *work);
BOOL TrainerApproach_WaitArrived(TrainerApproachWork *work);
BOOL TrainerApproach_TurnPlayer(TrainerApproachWork *work);
BOOL TrainerApproach_WaitPlayerTurn(TrainerApproachWork *work);
BOOL TrainerApproach_ResetTrainerMovement(TrainerApproachWork *work);
BOOL TrainerApproach_Finish(TrainerApproachWork *work);

#endif // POKEHEARTGOLD_MAP_OBJECT_BEHAVIOR_INTERNAL_H

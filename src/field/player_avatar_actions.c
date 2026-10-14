#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/maps.h"
#include "constants/player_avatar.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/sprites.h"
#include "constants/std_script.h"

#include "field/field_3d_object_task.h"
#include "field/field_control.h"
#include "field/field_effect_manager.h"
#include "field/field_follow_mon.h"

#include "field_bgm.h"
#include "field_metatile_query_internal.h"
#include "field_system.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "map_object_movement_commands.h"
#include "map_object_movement_commands_internal.h"
#include "map_object_movement_utilities.h"
#include "map_object_movement_utilities_internal.h"
#include "metatile_behavior.h"
#include "overlay_01.h"
#include "player_avatar_actions.h"
#include "field/map_object_render.h"
#include "field_surf_mount.h"
#include "overlay_02.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "sound_playback.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "task.h"

// The Pokemon shown using a field move: either the cut-in animation of the party Pokemon (FieldMoveCutIn_Start), or the
// following Pokemon, which is recalled first.
typedef struct FieldMoveAnim {
    BOOL useCutIn; // 0x00
    u32 gender;    // 0x04
    Pokemon *mon;  // 0x08
    void *cutIn;   // 0x0C
} FieldMoveAnim;   // size: 0x10

typedef struct PushBoulderWork {
    u32 state;                  // 0x00
    u32 direction;              // 0x04
    FieldSystem *fieldSystem;   // 0x08
    PlayerAvatar *playerAvatar; // 0x0C
    LocalMapObject *boulder;    // 0x10
} PushBoulderWork;              // size: 0x14

typedef struct SurfWork {
    int state;                  // 0x00
    int direction;              // 0x04
    int timer;                  // 0x08
    FieldMoveAnim anim;         // 0x0C
    FieldSystem *fieldSystem;   // 0x1C
    PlayerAvatar *playerAvatar; // 0x20
    LocalMapObject *mapObject;  // 0x24
    u32 surfMount;              // 0x28
} SurfWork;                     // size: 0x2C

typedef struct StopSurfingWork {
    u16 state;                  // 0x00
    u16 timer;                  // 0x02
    int direction;              // 0x04
    FieldSystem *fieldSystem;   // 0x08
    PlayerAvatar *playerAvatar; // 0x0C
    LocalMapObject *mapObject;  // 0x10
    u32 surfMount;              // 0x14
} StopSurfingWork;              // size: 0x18

typedef struct RockClimbWork {
    u16 state;                  // 0x00
    u16 climbState;             // 0x02, step to resume at while the next tile is rock climb too
    int direction;              // 0x04
    int timer;                  // 0x08
    FieldSystem *fieldSystem;   // 0x0C
    PlayerAvatar *playerAvatar; // 0x10
    LocalMapObject *mapObject;  // 0x14
    u32 effect;                 // 0x18
    FieldMoveAnim anim;         // 0x1C
    Field3dObjectTask *debris;   // 0x2C
} RockClimbWork;                // size: 0x30

// Waterfall (up and down) and Whirlpool.
typedef struct WaterfallWork {
    int state;                  // 0x00
    int direction;              // 0x04
    int counter;                // 0x08
    int targetX;                // 0x0C
    int targetY;                // 0x10
    int targetZ;                // 0x14
    int progress;               // 0x18
    VecFx32 step;               // 0x1C
    VecFx32 targetPos;          // 0x28
    FieldSystem *fieldSystem;   // 0x34
    PlayerAvatar *playerAvatar; // 0x38
    LocalMapObject *mapObject;  // 0x3C
    FieldMoveAnim anim;         // 0x40
    Field3dObjectTask *effect;  // 0x50
} WaterfallWork;                // size: 0x54

typedef struct PlayerSavingPoseWork {
    int unk00;                  // 0x00
    int state;                  // 0x04
    FieldSystem *fieldSystem;   // 0x08
    PlayerAvatar *playerAvatar; // 0x0C
} PlayerSavingPoseWork;         // size: 0x10

// Ice Path B1F boulders and the flag of the boulder that appears on the floor below once one is pushed into its hole.
typedef struct BoulderHole {
    u16 mapId;
    u16 x;
    u16 z;
    u16 objectId;
    u16 belowFlag;
} BoulderHole;

typedef int (*RockClimbStepFunc)(RockClimbWork *work, TaskManager *taskManager);
typedef int (*WaterfallStepFunc)(WaterfallWork *work);

// Defined in the player avatar movement and field effect modules, which have no header for them.
int PlayerAvatar_CheckReadyToMove(PlayerAvatar *avatar, int direction);
BOOL PlayerAvatar_IsReadyOrBumpingWall(PlayerAvatar *avatar);
int PlayerAvatar_GetMoveStateForDirection(PlayerAvatar *avatar, int direction);
u32 PlayerAvatar_GetMetatileBehaviorInDirection(PlayerAvatar *playerAvatar, int direction);
u32 PlayerAvatar_CheckCollision(PlayerAvatar *playerAvatar, LocalMapObject *object, int direction);
void SurfMountEffect_SetBobbing(u32 effect, int bobbing);
u32 RockClimbEffect_Create(LocalMapObject *mapObject, int x, int z, int direction, BOOL follow);
void RockClimbEffect_SetFollow(u32 effect, int follow);
Field3dObjectTask *WaterfallEffect_Create(FieldSystem *fieldSystem);
void WaterfallEffect_Delete(Field3dObjectTask *task);
Field3dObjectTask *WhirlpoolEffect_Create(FieldSystem *fieldSystem);
void WhirlpoolEffect_Delete(Field3dObjectTask *task);
Field3dObjectTask *RockClimbDebris_Create(FieldSystem *fieldSystem);
void RockClimbDebris_Delete(Field3dObjectTask *task);

void Field_PlayerAvatar_TransitionToWalking(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToCycling(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToSurfing(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToRocket(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionX0008(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToWatering(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToFishing(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToPoketch(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToSaving(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToHeal(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToLadder(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToRocketHeal(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToPokeathlon(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToApricornShake(PlayerAvatar *playerAvatar);
void Field_PlayerAvatar_TransitionToRocketSaving(PlayerAvatar *playerAvatar);
BOOL Field_PlayerAvatar_TryFieldMoveInDirection(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents);
BOOL Field_PlayerAvatar_TryPushBoulder(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision);
void Field_StartPushBoulderTask(int direction, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, LocalMapObject *boulder);
BOOL Task_PushBoulder(TaskManager *taskManager);
BOOL Field_TryDropBoulderIntoHole(FieldSystem *fieldSystem, LocalMapObject *boulder);
BOOL PushBoulder_Finish(TaskManager *taskManager, FieldSystem *fieldSystem, LocalMapObject *boulder);
void Field_StartSurfTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim);
BOOL Task_Surf(TaskManager *taskManager);
BOOL Field_PlayerAvatar_TryStopSurfing(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision);
void Field_StartStopSurfingTask(int direction, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar);
BOOL Task_StopSurfing(TaskManager *taskManager);
BOOL Field_PlayerAvatar_TryWaterfallUp(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision);
BOOL Field_PlayerAvatar_HandleForcedMovement(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int playerEvents);
void Field_StartRockClimbTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim);
BOOL Task_RockClimb(TaskManager *taskManager);
int RockClimb_StartCutIn(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_WaitCutIn(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_FollowMonReact(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_PlayFollowMonCry(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_WaitFollowMonCry(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_CreateEffect(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_StartJump(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_WaitJump(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_StartStep(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_CheckNextTile(RockClimbWork *work, TaskManager *taskManager);
int RockClimb_Finish(RockClimbWork *work, TaskManager *taskManager);
WaterfallWork *Waterfall_NewWork(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim);
void Field_StartWaterfallTask(FieldSystem *fieldSystem, u32 direction);
BOOL Task_Waterfall(TaskManager *taskManager);
int WaterfallUp_StartCutIn(WaterfallWork *work, TaskManager *taskManager);
int WaterfallUp_WaitCutIn(WaterfallWork *work, TaskManager *taskManager);
int WaterfallUp_Init(WaterfallWork *work, TaskManager *taskManager);
int WaterfallUp_Rise(WaterfallWork *work, TaskManager *taskManager);
int WaterfallUp_Climb(WaterfallWork *work, TaskManager *taskManager);
int WaterfallDown_StartCutIn(WaterfallWork *work, TaskManager *taskManager);
int WaterfallDown_WaitCutIn(WaterfallWork *work, TaskManager *taskManager);
int WaterfallDown_Init(WaterfallWork *work, TaskManager *taskManager);
int WaterfallDown_Advance(WaterfallWork *work, TaskManager *taskManager);
int WaterfallDown_Descend(WaterfallWork *work, TaskManager *taskManager);
void Field_StartWhirlpoolTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim);
BOOL Task_Whirlpool(TaskManager *taskManager);
int Whirlpool_StartCutIn(WaterfallWork *work, TaskManager *taskManager);
int Whirlpool_WaitCutIn(WaterfallWork *work, TaskManager *taskManager);
int Whirlpool_CreateEffect(WaterfallWork *work, TaskManager *taskManager);
int Whirlpool_StartStep(WaterfallWork *work, TaskManager *taskManager);
int Whirlpool_CheckNextTile(WaterfallWork *work);
void PlayerMovementSaving_Update(SysTask *task, void *data);
void FieldMoveAnim_Init(FieldSystem *fieldSystem, Pokemon *mon, FieldMoveAnim *anim);
void FieldMoveAnim_Start(FieldSystem *fieldSystem, FieldMoveAnim *anim);
BOOL FieldMoveAnim_CheckFinished(FieldMoveAnim *anim);
void Field_PlayerAvatar_SetSprite(PlayerAvatar *playerAvatar, u32 sprite);
LocalMapObject *Field_PlayerAvatar_GetFacingMapObject(PlayerAvatar *playerAvatar, int direction);
void *FieldMoveTask_AllocWork(u32 size);
void FieldMoveTask_FreeWork(void *work);
Pokemon *Field_GetPartyMon(FieldSystem *fieldSystem, int partySlot);

extern const WaterfallStepFunc sWhirlpoolSteps[5];
extern const WaterfallStepFunc sWaterfallUpSteps[5];
extern const WaterfallStepFunc sWaterfallDownSteps[5];
extern const RockClimbStepFunc sRockClimbCutInSteps[8];
extern const RockClimbStepFunc sRockClimbFollowMonSteps[9];
extern const BoulderHole sIcePathBoulderHoles[4];
extern void (*const sPlayerAvatarBitUpdateFuncs[15])(PlayerAvatar *);

void Field_PlayerAvatar_OrrTransitionFlags(PlayerAvatar *playerAvatar, int flags) {
    PlayerAvatar_SetTransitionFlagsBits(playerAvatar, flags);
}

#pragma force_active on
void Field_PlayerAvatar_ApplyTransitionFlags(PlayerAvatar *avatar) {
    u32 i = 0;
    u32 flags = PlayerAvatar_GetTransitionFlags(avatar);
    for (; i < 15; i++) {
        if (flags & 1) {
            sPlayerAvatarBitUpdateFuncs[i](avatar);
        }
        flags >>= 1;
    }
    PlayerAvatar_SetTransitionFlags(avatar, 0);
}
#pragma force_active reset

void Field_PlayerAvatar_TransitionToWalking(PlayerAvatar *playerAvatar) {
    u32 surfMount;
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_WALKING, PlayerAvatar_GetGender(playerAvatar)));
    PlayerAvatar_SetState(playerAvatar, PLAYER_STATE_WALKING);
    PlayerAvatar_ClearSpeedAndFlag2(playerAvatar);
    surfMount = PlayerAvatar_GetSurfMountEffect(playerAvatar);
    if (surfMount != 0) {
        FieldEffectManager_DestroyEffect(surfMount);
    }
    PlayerAvatar_SetSurfMountEffect(playerAvatar, 0);
}

void Field_PlayerAvatar_TransitionToCycling(PlayerAvatar *playerAvatar) {
    u32 surfMount;
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_CYCLING, PlayerAvatar_GetGender(playerAvatar)));
    PlayerAvatar_SetState(playerAvatar, PLAYER_STATE_CYCLING);
    PlayerAvatar_ClearSpeedAndFlag2(playerAvatar);
    surfMount = PlayerAvatar_GetSurfMountEffect(playerAvatar);
    if (surfMount != 0) {
        FieldEffectManager_DestroyEffect(surfMount);
    }
    PlayerAvatar_SetSurfMountEffect(playerAvatar, 0);
    PlaySE(SEQ_SE_DP_JITENSYA);
}

void Field_PlayerAvatar_TransitionToSurfing(PlayerAvatar *playerAvatar) {
    u32 surfMount;
    int direction;
    LocalMapObject *mapObject;
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_SURFING, PlayerAvatar_GetGender(playerAvatar)));
    PlayerAvatar_SetState(playerAvatar, PLAYER_STATE_SURFING);
    PlayerAvatar_ClearSpeedAndFlag2(playerAvatar);
    surfMount = PlayerAvatar_GetSurfMountEffect(playerAvatar);
    if (surfMount != 0) {
        FieldEffectManager_DestroyEffect(surfMount);
    }
    PlayerAvatar_SetSurfMountEffect(playerAvatar, 0);
    direction = PlayerAvatar_GetFacingDirection(playerAvatar);
    mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    MapObject_GetFieldSystem(mapObject);
    PlayerAvatar_SetSurfMountEffect(playerAvatar, SurfMountEffect_Create(mapObject, 0, 0, direction, 1));
}

void Field_PlayerAvatar_TransitionToRocket(PlayerAvatar *playerAvatar) {
    u32 surfMount;
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_ROCKET, PlayerAvatar_GetGender(playerAvatar)));
    PlayerAvatar_SetState(playerAvatar, PLAYER_STATE_ROCKET);
    PlayerAvatar_ClearSpeedAndFlag2(playerAvatar);
    surfMount = PlayerAvatar_GetSurfMountEffect(playerAvatar);
    if (surfMount != 0) {
        FieldEffectManager_DestroyEffect(surfMount);
    }
    PlayerAvatar_SetSurfMountEffect(playerAvatar, 0);
}

void Field_PlayerAvatar_TransitionX0008(PlayerAvatar *playerAvatar) {
    PlayerAvatar_SetFlag1(playerAvatar, TRUE);
}

void Field_PlayerAvatar_TransitionToWatering(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_WATERING, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToFishing(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_FISHING, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToPoketch(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_POKETCH, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToSaving(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_SAVING, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToHeal(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_HEAL, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToLadder(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_LADDER, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToRocketHeal(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_ROCKET_HEAL, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToPokeathlon(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_POKEATHLON, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToApricornShake(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_APRICORN_SHAKE, PlayerAvatar_GetGender(playerAvatar)));
}

void Field_PlayerAvatar_TransitionToRocketSaving(PlayerAvatar *playerAvatar) {
    Field_PlayerAvatar_SetSprite(playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_ROCKET_SAVING, PlayerAvatar_GetGender(playerAvatar)));
}

BOOL Field_PlayerAvatar_TryFieldMoveStep(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents) {
    if (PlayerAvatar_CheckForcedMovement(playerAvatar) == TRUE) {
        if (Field_PlayerAvatar_HandleForcedMovement(fieldSystem, playerAvatar, playerEvents) == TRUE) {
            return TRUE;
        }
        return FALSE;
    }
    if (!PlayerAvatar_CheckReadyToMove(playerAvatar, direction)) {
        return FALSE;
    }
    if (direction == -1) {
        return FALSE;
    }
    if (Field_PlayerAvatar_TryFieldMoveInDirection(fieldSystem, playerAvatar, direction, playerEvents) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL Field_PlayerAvatar_TryFieldMoveInDirection(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int playerEvents) {
    int collision = PlayerAvatar_GetMoveStateForDirection(playerAvatar, direction);
    if (playerEvents & PLAYER_EVENT_USED_STRENGTH) {
        if (Field_PlayerAvatar_TryPushBoulder(fieldSystem, playerAvatar, direction, collision) == TRUE) {
            return TRUE;
        }
    }
    if (playerEvents & PLAYER_EVENT_USED_WATERFALL) {
        if (Field_PlayerAvatar_TryWaterfallUp(fieldSystem, playerAvatar, direction, collision) == TRUE) {
            return TRUE;
        }
    }
    if (Field_PlayerAvatar_TryStopSurfing(fieldSystem, playerAvatar, direction, collision) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL Field_PlayerAvatar_TryPushBoulder(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision) {
    LocalMapObject *boulder;
    if (collision != 1) {
        return FALSE;
    }
    boulder = Field_PlayerAvatar_GetFacingMapObject(playerAvatar, direction);
    if (boulder == NULL) {
        return FALSE;
    }
    if (MapObject_GetSpriteID(boulder) != SPRITE_ROCK) {
        return FALSE;
    }
    if ((MapObject_CheckCollisionInDirection(boulder, direction) & ~1) != 0) {
        return FALSE;
    }
    Field_StartPushBoulderTask(direction, fieldSystem, playerAvatar, boulder);
    return TRUE;
}

void Field_StartPushBoulderTask(int direction, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, LocalMapObject *boulder) {
    PushBoulderWork *work = FieldMoveTask_AllocWork(sizeof(PushBoulderWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    work->boulder = boulder;
    work->playerAvatar = playerAvatar;
    FieldSystem_CreateTask(fieldSystem, Task_PushBoulder, work);
}

#pragma force_active on
BOOL Task_PushBoulder(TaskManager *taskManager) {
    PushBoulderWork *work = TaskManager_GetEnvironment(taskManager);
    LocalMapObject *player = PlayerAvatar_GetMapObject(work->playerAvatar);
    LocalMapObject *boulder = work->boulder;
    switch (work->state) {
    case 0:
        if (!MapObject_AreBitsSetForMovementScriptInit(boulder)) {
            break;
        }
        if (!MapObject_AreBitsSetForMovementScriptInit(player) && !PlayerAvatar_IsReadyOrBumpingWall(work->playerAvatar)) {
            break;
        }
        MapObject_SetHeldMovement(boulder, GetMovementForDirection(work->direction, 8));
        MapObject_SetHeldMovement(player, GetMovementForDirection(work->direction, 32));
        PlaySE(SEQ_SE_DP_UG_023);
        work->state++;
        break;
    case 1:
        if (!MapObject_IsMovementPaused(boulder)) {
            break;
        }
        if (!MapObject_IsMovementPaused(player)) {
            break;
        }
        MapObject_ClearHeldMovementIfActive(boulder);
        MapObject_ClearHeldMovementIfActive(player);
        work->state++;
        // fallthrough
    case 2: {
        FieldSystem *fieldSystem = work->fieldSystem;
        FieldMoveTask_FreeWork(work);
        if (fieldSystem->location->mapId == MAP_ICE_PATH_B1F) {
            return PushBoulder_Finish(taskManager, fieldSystem, boulder);
        }
        return TRUE;
    }
    }
    return FALSE;
}

BOOL Field_TryDropBoulderIntoHole(FieldSystem *fieldSystem, LocalMapObject *boulder) {
    u32 i;
    for (i = 0; i < 4; i++) {
        if (sIcePathBoulderHoles[i].mapId == MapObject_GetMapID(boulder) && sIcePathBoulderHoles[i].objectId == MapObject_GetID(boulder) && sIcePathBoulderHoles[i].x == MapObject_GetXCoord(boulder) && sIcePathBoulderHoles[i].z == MapObject_GetZCoord(boulder)) {
            FieldSystem_FlagSet(fieldSystem, (u16)MapObject_GetEventFlag(boulder));
            MapObject_Delete(boulder);
            {
                const BoulderHole *hole = sIcePathBoulderHoles + i;
                FieldSystem_FlagClear(fieldSystem, hole->belowFlag);
            }
            return TRUE;
        }
    }
    return FALSE;
}
#pragma force_active reset

BOOL PushBoulder_Finish(TaskManager *taskManager, FieldSystem *fieldSystem, LocalMapObject *boulder) {
    if (Field_TryDropBoulderIntoHole(fieldSystem, boulder) == FALSE) {
        return TRUE;
    }
    StartScriptFromMenu(taskManager, std_boulder_fell_down, NULL);
    return FALSE;
}

#pragma force_active on
void Field_StartSurfTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim) {
    SurfWork *work = FieldMoveTask_AllocWork(sizeof(SurfWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    work->playerAvatar = fieldSystem->playerAvatar;
    work->mapObject = PlayerAvatar_GetMapObject(work->playerAvatar);
    work->anim = *anim;
    TaskManager_Call(fieldSystem->taskman, Task_Surf, work);
}

#pragma force_active reset

void CallFieldTask_Surf(TaskManager *taskManager, int direction, int partySlot) {
    u32 state;
    FieldMoveAnim anim;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveAnim_Init(fieldSystem, Field_GetPartyMon(fieldSystem, partySlot), &anim);
    state = PlayerAvatar_GetState(fieldSystem->playerAvatar);
    if (state - 1 <= 1) {
        anim.useCutIn = TRUE;
    } else if (FollowMon_IsNextToPlayer(fieldSystem) != FALSE && partySlot == FieldSystem_GetLeadMonIndex(fieldSystem)) {
        anim.useCutIn = FALSE;
    }
    Field_StartSurfTask(fieldSystem, direction, &anim);
}

BOOL Field_PlayerCanSurfOnTile(PlayerAvatar *playerAvatar, int currentTile, int facingTile) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    if (MetatileBehavior_IsSurfableWater(facingTile) == TRUE) {
        if (MetatileBehavior_IsBridge(currentTile) == TRUE || MetatileBehavior_IsBridgeStart(currentTile) == TRUE) {
            if (MapObject_CheckFlag28(mapObject) == TRUE) {
                return FALSE;
            }
        }
        if (MetatileBehavior_Is34(currentTile) == TRUE) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on
BOOL Task_Surf(TaskManager *taskManager) {
    SurfWork *work = TaskManager_GetEnvironment(taskManager);
    switch (work->state) {
    case 0:
        if (!CheckFlag99A(Save_VarsFlags_Get(work->fieldSystem->saveData)) && !SndRadio_GetSeqNo()) {
            FieldBGM_SetOverride(work->fieldSystem, 0);
            FieldBGM_TryFadeOut(work->fieldSystem, SEQ_GS_NAMINORI, 1);
        }
        work->state++;
        work->state++;
        break;
    case 1:
        if (++work->timer > 10) {
            work->timer = 0;
            work->state++;
        }
        break;
    case 2:
        if (work->anim.useCutIn == TRUE) {
            FieldMoveAnim_Start(work->fieldSystem, &work->anim);
            work->state = 5;
        } else {
            u8 count;
            if (FieldSystem_FirstAliveMonHasType(work->fieldSystem, TYPE_WATER)) {
                FollowMonMood_Add(work->fieldSystem->followMonMood, 1);
                count = 2;
            } else {
                count = 1;
            }
            FollowMon_JumpOnSpot(work->fieldSystem, count);
            work->state = 3;
        }
        break;
    case 3: {
        int species = GetMonData(work->anim.mon, MON_DATA_SPECIES, NULL);
        int form = GetMonData(work->anim.mon, MON_DATA_FORM, NULL);
        PlayCry((u16)species, (u8)form);
        work->state = 4;
        break;
    }
    case 4:
        if (!IsCryFinished()) {
            FollowMon_StartHideForFieldMove(taskManager);
            work->state = 6;
        }
        break;
    case 5:
        if (FieldMoveAnim_CheckFinished(&work->anim) == TRUE) {
            FollowMon_StartRecall(work->fieldSystem);
            work->state = 6;
        }
        break;
    case 6: {
        int x = PlayerAvatar_GetXCoord(work->playerAvatar);
        int dx = GetDeltaXByFacingDirection(work->direction);
        int z = PlayerAvatar_GetZCoord(work->playerAvatar);
        int dz = GetDeltaYByFacingDirection(work->direction);
        work->surfMount = SurfMountEffect_Create(work->mapObject, x + dx, z + dz, work->direction, 0);
        PlayerAvatar_SetSurfMountEffect(work->playerAvatar, work->surfMount);
        PlayerAvatar_SetState(work->playerAvatar, PLAYER_STATE_SURFING);
        work->state++;
        break;
    }
    case 7:
        if (++work->timer > 10) {
            Field_PlayerAvatar_SetSprite(work->playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_WALKING, PlayerAvatar_GetGender(work->playerAvatar)));
            work->timer = 0;
            work->state++;
        }
        break;
    case 8:
        if (MapObject_AreBitsSetForMovementScriptInit(work->mapObject) == TRUE) {
            MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, 52));
            work->state++;
        }
        break;
    case 9:
        if (MapObject_IsMovementPaused(work->mapObject)) {
            MapObject_ClearHeldMovementIfActive(work->mapObject);
            SurfMountEffect_SetBobbing(work->surfMount, TRUE);
            Field_PlayerAvatar_SetSprite(work->playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_SURFING, PlayerAvatar_GetGender(work->playerAvatar)));
            if (FollowMon_IsActive(work->fieldSystem)) {
                FollowMon_MoveToPlayer(work->fieldSystem, (u8)work->direction);
                MapObject_ChangeMovement(FollowMon_GetMapObject(work->fieldSystem), 56);
            }
            FieldSystem_ProcessSoundplate(work->fieldSystem, FALSE);
            FieldMoveTask_FreeWork(work);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL Field_PlayerAvatar_TryStopSurfing(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision) {
    u32 result;

    if (collision != 1 || PlayerAvatar_GetState(playerAvatar) != PLAYER_STATE_SURFING) {
        return FALSE;
    }
    result = PlayerAvatar_CheckCollision(playerAvatar, PlayerAvatar_GetMapObject(playerAvatar), direction);
    if (result == 0x20) {
        return FALSE;
    }
    if ((result & ~0x20) != 0) {
        return FALSE;
    }
    Field_StartStopSurfingTask(direction, fieldSystem, playerAvatar);
    return TRUE;
}

void Field_StartStopSurfingTask(int direction, FieldSystem *fieldSystem, PlayerAvatar *playerAvatar) {
    StopSurfingWork *work;

    work = FieldMoveTask_AllocWork(sizeof(StopSurfingWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    work->playerAvatar = playerAvatar;
    work->mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    work->surfMount = PlayerAvatar_GetSurfMountEffect(playerAvatar);
    if (work->surfMount == 0) {
        GF_AssertFail();
    }
    FieldSystem_CreateTask(fieldSystem, Task_StopSurfing, work);
}

BOOL Task_StopSurfing(TaskManager *taskManager) {
    StopSurfingWork *work = TaskManager_GetEnvironment(taskManager);

    switch (work->state) {
    case 0:
        if (MapObject_AreBitsSetForMovementScriptInit(work->mapObject) == TRUE) {
            Field_PlayerAvatar_SetSprite(work->playerAvatar, PlayerAvatar_GetSpriteByStateAndGender(PLAYER_STATE_WALKING, PlayerAvatar_GetGender(work->playerAvatar)));
            MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, 0x34));
            SurfMountEffect_SetBobbing(work->surfMount, FALSE);
            work->state++;
        }
        break;
    case 1:
        if (MapObject_IsMovementPaused(work->mapObject)) {
            MapObject_ClearHeldMovementIfActive(work->mapObject);
            FieldEffectManager_DestroyEffect(work->surfMount);
            PlayerAvatar_SetSurfMountEffect(work->playerAvatar, 0);
            PlayerAvatar_SetState(work->playerAvatar, PLAYER_STATE_WALKING);
            FieldSystem_ProcessSoundplate(work->fieldSystem, FALSE);
            if (FollowMon_IsActive(work->fieldSystem)) {
                FollowMon_MoveToPlayer(work->fieldSystem, (u8)work->direction);
                FollowMon_HideUntilStep(FollowMon_GetMapObject(work->fieldSystem), TRUE);
                FollowMon_SetFacingDirection(work->fieldSystem, DIR_SOUTH);
                MapObject_ChangeMovement(FollowMon_GetMapObject(work->fieldSystem), 0x30);
            }
            if (!CheckFlag99A(Save_VarsFlags_Get(work->fieldSystem->saveData))) {
                FieldBGM_TryFadeOut(work->fieldSystem, FieldBGM_GetForMapHeader(work->fieldSystem, work->fieldSystem->location->mapId), 4);
            }
            if (SndRadio_GetSeqNo() != 0) {
                work->timer = 0x28;
                work->state++;
            } else {
                FieldMoveTask_FreeWork(work);
                return TRUE;
            }
        }
        break;
    case 2:
        if (SndRadio_GetSeqNo() == 0) {
            FieldSystem_ProcessSoundplate(work->fieldSystem, TRUE);
            FieldMoveTask_FreeWork(work);
            return TRUE;
        }
        work->timer--;
        if (work->timer == 0) {
            FieldSystem_ProcessSoundplate(work->fieldSystem, TRUE);
            FieldMoveTask_FreeWork(work);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL Field_PlayerAvatar_TryWaterfallUp(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int direction, int collision) {
    if (direction != 1 || collision != 1 || PlayerAvatar_GetState(playerAvatar) != PLAYER_STATE_SURFING) {
        return FALSE;
    }
    if (!MetatileBehavior_IsWaterfall((u8)PlayerAvatar_GetMetatileBehaviorInDirection(playerAvatar, direction))) {
        return FALSE;
    }
    Field_StartWaterfallTask(fieldSystem, direction);
    return TRUE;
}

BOOL Field_PlayerAvatar_HandleForcedMovement(FieldSystem *fieldSystem, PlayerAvatar *playerAvatar, int playerEvents) {
    MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(playerAvatar));
    return FALSE;
}

void Field_StartRockClimbTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim) {
    PlayerAvatar *playerAvatar;
    RockClimbWork *work = FieldMoveTask_AllocWork(sizeof(RockClimbWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    playerAvatar = fieldSystem->playerAvatar;
    work->playerAvatar = playerAvatar;
    work->mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    work->anim = *anim;
    if (work->anim.useCutIn == TRUE) {
        work->climbState = 5;
    } else {
        work->climbState = 6;
    }
    TaskManager_Call(fieldSystem->taskman, Task_RockClimb, work);
}

void CallFieldTask_RockClimb(TaskManager *taskManager, int direction, int partySlot) {
    u32 state;
    FieldMoveAnim anim;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveAnim_Init(fieldSystem, Field_GetPartyMon(fieldSystem, partySlot), &anim);
    state = PlayerAvatar_GetState(fieldSystem->playerAvatar);
    if (state - 1 <= 1) {
        anim.useCutIn = TRUE;
    } else if (FollowMon_IsNextToPlayer(fieldSystem) != FALSE && partySlot == FieldSystem_GetLeadMonIndex(fieldSystem)) {
        anim.useCutIn = FALSE;
    }
    Field_StartRockClimbTask(fieldSystem, direction, &anim);
}

BOOL MetatileBehavior_IsRockClimbInDirection(int facingTile, int facingDirection) {
    switch (facingDirection) {
    case DIR_NORTH:
    case DIR_SOUTH:
        if (MetatileBehavior_IsRockClimbNorthSouth((u8)facingTile) == TRUE) {
            return TRUE;
        }
        break;
    case DIR_WEST:
    case DIR_EAST:
        if (MetatileBehavior_IsRockClimbEastWest((u8)facingTile) == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#pragma force_active on
BOOL Task_RockClimb(TaskManager *taskManager) {
    RockClimbWork *work = TaskManager_GetEnvironment(taskManager);
    int result;
    do {
        if (work->anim.useCutIn == TRUE) {
            result = sRockClimbCutInSteps[work->state](work, taskManager);
        } else {
            result = sRockClimbFollowMonSteps[work->state](work, taskManager);
        }
    } while (result == 2);
    if (result == 1) {
        FollowMon_MoveToPlayer(work->fieldSystem, (u8)work->direction);
        FollowMon_HideUntilStep(FollowMon_GetMapObject(work->fieldSystem), TRUE);
        FollowMon_SetFacingDirection(work->fieldSystem, DIR_SOUTH);
        FieldMoveTask_FreeWork(work);
    }
    return result;
}
#pragma force_active reset

int RockClimb_StartCutIn(RockClimbWork *work, TaskManager *taskManager) {
    FieldMoveAnim_Start(work->fieldSystem, &work->anim);
    work->state++;
    return 0;
}

int RockClimb_WaitCutIn(RockClimbWork *work, TaskManager *taskManager) {
    if (FieldMoveAnim_CheckFinished(&work->anim) == TRUE) {
        FollowMon_StartRecall(work->fieldSystem);
        work->state++;
    }
    return 0;
}

#pragma force_active on
int RockClimb_FollowMonReact(RockClimbWork *work, TaskManager *taskManager) {
    u8 count;
    if (FieldSystem_FirstAliveMonHasType(work->fieldSystem, TYPE_NORMAL)) {
        FollowMonMood_Add(work->fieldSystem->followMonMood, 1);
        count = 2;
    } else {
        count = 1;
    }
    FollowMon_JumpOnSpot(work->fieldSystem, count);
    work->state++;
    return 0;
}

int RockClimb_PlayFollowMonCry(RockClimbWork *work, TaskManager *taskManager) {
    int species = GetMonData(work->anim.mon, MON_DATA_SPECIES, NULL);
    int form = GetMonData(work->anim.mon, MON_DATA_FORM, NULL);
    PlayCry((u16)species, (u8)form);
    work->state++;
    return 0;
}
#pragma force_active reset

int RockClimb_WaitFollowMonCry(RockClimbWork *work, TaskManager *taskManager) {
    if (IsCryFinished()) {
        return 0;
    }
    FollowMon_StartHideForFieldMove(taskManager);
    work->state++;
    return 0;
}

#pragma force_active on
int RockClimb_CreateEffect(RockClimbWork *work, TaskManager *taskManager) {
    int x = PlayerAvatar_GetXCoord(work->playerAvatar);
    int dx = GetDeltaXByFacingDirection(work->direction);
    int z = PlayerAvatar_GetZCoord(work->playerAvatar);
    int dz = GetDeltaYByFacingDirection(work->direction);
    work->effect = RockClimbEffect_Create(work->mapObject, x + dx, z + dz, work->direction, FALSE);
    if (FollowMon_IsActive(work->fieldSystem)) {
        MapObject_UnpauseMovement(FollowMon_GetMapObject(work->fieldSystem));
    }
    work->debris = RockClimbDebris_Create(work->fieldSystem);
    PlaySE(SEQ_SE_DP_UG_023);
    work->state++;
    return 0;
}
#pragma force_active reset

int RockClimb_StartJump(RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->mapObject) == TRUE) {
        MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, 0x34));
        work->state++;
    }
    return 0;
}

int RockClimb_WaitJump(RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_IsMovementPaused(work->mapObject) == TRUE) {
        work->state++;
        RockClimbEffect_SetFollow(work->effect, TRUE);
    }
    return 0;
}

int RockClimb_StartStep(RockClimbWork *work, TaskManager *taskManager) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->mapObject) == TRUE) {
        MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, MOVEMENT_RUN_UP));
        work->state++;
    }
    return 0;
}

#pragma force_active on
int RockClimb_CheckNextTile(RockClimbWork *work, TaskManager *taskManager) {
    int direction;
    if (!MapObject_IsMovementPaused(work->mapObject)) {
        return 0;
    }
    direction = MapObject_GetNextFacingDirection(work->mapObject);
    if (MetatileBehavior_IsRockClimbInDirection((u8)MapObject_GetMetatileBehaviorInDirection(work->mapObject, direction), direction) == TRUE) {
        work->state = work->climbState;
        return 2;
    }
    MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, 52));
    work->timer = 0;
    work->state++;
    RockClimbEffect_SetFollow(work->effect, FALSE);
    RockClimbDebris_Delete(work->debris);
    work->debris = NULL;
    return 0;
}
#pragma force_active reset

int RockClimb_Finish(RockClimbWork *work, TaskManager *taskManager) {
    if (!MapObject_IsMovementPaused(work->mapObject)) {
        return 0;
    }
    MapObject_ClearHeldMovementIfActive(work->mapObject);
    FieldEffectManager_DestroyEffect(work->effect);
    return 1;
}

WaterfallWork *Waterfall_NewWork(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim) {
    WaterfallWork *work = FieldMoveTask_AllocWork(sizeof(WaterfallWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    work->playerAvatar = fieldSystem->playerAvatar;
    work->mapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    if (anim != NULL) {
        work->anim = *anim;
    }
    return work;
}

void Field_StartWaterfallTask(FieldSystem *fieldSystem, u32 direction) {
    FieldSystem_CreateTask(fieldSystem, Task_Waterfall, Waterfall_NewWork(fieldSystem, direction, NULL));
}

void CallFieldTask_Waterfall(TaskManager *taskManager, int direction, int partySlot) {
    FieldMoveAnim anim;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveAnim_Init(fieldSystem, Field_GetPartyMon(fieldSystem, partySlot), &anim);
    TaskManager_Call(taskManager, Task_Waterfall, Waterfall_NewWork(fieldSystem, direction, &anim));
}

#pragma force_active on
BOOL Task_Waterfall(TaskManager *taskManager) {
    WaterfallWork *work = TaskManager_GetEnvironment(taskManager);
    int result;
    do {
        if (work->direction == 0) {
            result = sWaterfallUpSteps[work->state](work);
        } else {
            result = sWaterfallDownSteps[work->state](work);
        }
    } while (result == 2);
    if (result == 1) {
        FollowMon_MoveToPlayer(work->fieldSystem, (u8)work->direction);
        FieldMoveTask_FreeWork(work);
        return TRUE;
    }
    return FALSE;
}
#pragma force_active reset

int WaterfallUp_StartCutIn(WaterfallWork *work, TaskManager *taskManager) {
    if (work->anim.useCutIn == TRUE) {
        FieldMoveAnim_Start(work->fieldSystem, &work->anim);
        work->state++;
        return 0;
    }
    work->state = 2;
    return 1;
}

int WaterfallUp_WaitCutIn(WaterfallWork *work, TaskManager *taskManager) {
    if (FieldMoveAnim_CheckFinished(&work->anim) == TRUE) {
        PlaySE(SEQ_SE_DP_FW463);
        work->state++;
    }
    return 0;
}

int WaterfallUp_Init(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    VecFx32 step;
    u8 kind;
    int x = MapObject_GetXCoord(work->mapObject) + (GetDeltaXByFacingDirection(DIR_NORTH) << 1);
    int z = MapObject_GetZCoord(work->mapObject) + (GetDeltaYByFacingDirection(DIR_NORTH) << 1);
    SetPositionToTileCenter(x, z, &work->targetPos);
    work->targetPos.y = Terrain_CalcHeightWithMode(work->fieldSystem, 1, work->targetPos.y, work->targetPos.x, work->targetPos.z, &kind);
    GF_ASSERT(kind != 0);
    work->targetX = x;
    work->targetY = (work->targetPos.y >> 3) / FX32_ONE;
    work->targetZ = z;
    MapObject_CopyPositionVector(work->mapObject, &position);
    GF_ASSERT(position.z > work->targetPos.z);
    GF_ASSERT(position.y < work->targetPos.y);
    step.x = 0;
    step.y = work->targetPos.y - position.y;
    step.z = work->targetPos.z - position.z;
    step.y /= 64;
    step.z /= 64;
    work->step = step;
    work->effect = WaterfallEffect_Create(work->fieldSystem);
    work->state++;
    return 0;
}

int WaterfallUp_Rise(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    MapObject_CopyPositionVector(work->mapObject, &position);
    position.y += work->step.y;
    if (position.y > work->targetPos.y) {
        position.y = work->targetPos.y;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter >= 32) {
        work->counter = 0;
        work->state++;
    }
    return 0;
}

int WaterfallUp_Climb(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    MapObject_CopyPositionVector(work->mapObject, &position);
    position.y += work->step.y;
    if (position.y > work->targetPos.y) {
        position.y = work->targetPos.y;
    }
    position.z += work->step.z;
    if (position.z < work->targetPos.z) {
        position.z = work->targetPos.z;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter < 64) {
        return 0;
    }
    GF_ASSERT(position.z == work->targetPos.z);
    GF_ASSERT(position.y == work->targetPos.y);
    MapObject_SetCurrentX(work->mapObject, work->targetX);
    MapObject_SetCurrentY(work->mapObject, work->targetY);
    MapObject_SetCurrentZ(work->mapObject, work->targetZ);
    MapObject_SetPreviousCoordsToCurrent(work->mapObject);
    WaterfallEffect_Delete(work->effect);
    return 1;
}

int WaterfallDown_StartCutIn(WaterfallWork *work, TaskManager *taskManager) {
    if (work->anim.useCutIn == TRUE) {
        FieldMoveAnim_Start(work->fieldSystem, &work->anim);
        work->state++;
        return 0;
    }
    work->state = 2;
    return 2;
}

int WaterfallDown_WaitCutIn(WaterfallWork *work, TaskManager *taskManager) {
    if (FieldMoveAnim_CheckFinished(&work->anim) == TRUE) {
        PlaySE(SEQ_SE_DP_FW463);
        work->state++;
    }
    return 0;
}

int WaterfallDown_Init(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    VecFx32 step;
    int x = MapObject_GetXCoord(work->mapObject) + (GetDeltaXByFacingDirection(DIR_SOUTH) << 1);
    int z = MapObject_GetZCoord(work->mapObject) + (GetDeltaYByFacingDirection(DIR_SOUTH) << 1);
    SetPositionToTileCenter(x, z, &work->targetPos);
    FieldSystem_SnapToTerrainHeight(work->fieldSystem, &work->targetPos);
    work->progress = 0;
    work->targetX = x;
    work->targetY = (work->targetPos.y >> 3) / FX32_ONE;
    work->targetZ = z;
    MapObject_CopyPositionVector(work->mapObject, &position);
    if (position.z >= work->targetPos.z) {
        GF_AssertFail();
    }
    if (position.y <= work->targetPos.y) {
        GF_AssertFail();
    }
    step.x = 0;
    step.y = work->targetPos.y - position.y;
    step.z = work->targetPos.z - position.z;
    step.y /= 64;
    step.z /= 64;
    work->step = step;
    work->effect = WaterfallEffect_Create(work->fieldSystem);
    work->state++;
    return 0;
}

int WaterfallDown_Advance(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    MapObject_CopyPositionVector(work->mapObject, &position);
    position.z += work->step.z;
    if (position.z > work->targetPos.z) {
        position.z = work->targetPos.z;
    } else {
        work->progress += work->step.z;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter >= 32) {
        work->counter = 0;
        work->state++;
    }
    return 0;
}

int WaterfallDown_Descend(WaterfallWork *work, TaskManager *taskManager) {
    VecFx32 position;
    MapObject_CopyPositionVector(work->mapObject, &position);
    position.y += work->step.y;
    if (position.y < work->targetPos.y) {
        position.y = work->targetPos.y;
    }
    position.z += work->step.z;
    if (position.z > work->targetPos.z) {
        position.z = work->targetPos.z;
    } else {
        work->progress += work->step.z;
    }
    MapObject_SetPositionVector(work->mapObject, &position);
    work->counter++;
    if (work->counter < 64) {
        return 0;
    }
    if (position.z != work->targetPos.z) {
        GF_AssertFail();
    }
    if (position.y != work->targetPos.y) {
        GF_AssertFail();
    }
    MapObject_SetCurrentX(work->mapObject, work->targetX);
    MapObject_SetCurrentY(work->mapObject, work->targetY);
    MapObject_SetCurrentZ(work->mapObject, work->targetZ);
    MapObject_SetPreviousCoordsToCurrent(work->mapObject);
    WaterfallEffect_Delete(work->effect);
    return 1;
}

void Field_StartWhirlpoolTask(FieldSystem *fieldSystem, int direction, FieldMoveAnim *anim) {
    WaterfallWork *work = FieldMoveTask_AllocWork(sizeof(WaterfallWork));
    work->direction = direction;
    work->fieldSystem = fieldSystem;
    work->playerAvatar = fieldSystem->playerAvatar;
    work->mapObject = PlayerAvatar_GetMapObject(work->playerAvatar);
    work->anim = *anim;
    TaskManager_Call(fieldSystem->taskman, Task_Whirlpool, work);
}

void CallFieldTask_Whirlpool(TaskManager *taskManager, int direction, int partySlot) {
    FieldMoveAnim anim;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskManager);
    FieldMoveAnim_Init(fieldSystem, Field_GetPartyMon(fieldSystem, partySlot), &anim);
    Field_StartWhirlpoolTask(fieldSystem, direction, &anim);
}

#pragma force_active on
BOOL Task_Whirlpool(TaskManager *taskManager) {
    int result;
    WaterfallWork *work = TaskManager_GetEnvironment(taskManager);
    do {
        result = sWhirlpoolSteps[work->state](work);
    } while (result == 2);
    if (result == 1) {
        FollowMon_MoveToPlayer(work->fieldSystem, (u8)work->direction);
        FieldMoveTask_FreeWork(work);
    }
    return result;
}
#pragma force_active reset

int Whirlpool_StartCutIn(WaterfallWork *work, TaskManager *taskManager) {
    FieldMoveAnim_Start(work->fieldSystem, &work->anim);
    work->state++;
    return 0;
}

int Whirlpool_WaitCutIn(WaterfallWork *work, TaskManager *taskManager) {
    if (FieldMoveAnim_CheckFinished(&work->anim) == TRUE) {
        work->state++;
    }
    return 0;
}

int Whirlpool_CreateEffect(WaterfallWork *work, TaskManager *taskManager) {
    if (FollowMon_IsActive(work->fieldSystem)) {
        MapObject_UnpauseMovement(FollowMon_GetMapObject(work->fieldSystem));
    }
    work->effect = WhirlpoolEffect_Create(work->fieldSystem);
    PlaySE(SEQ_SE_DP_FW463);
    work->state++;
    return 0;
}

int Whirlpool_StartStep(WaterfallWork *work, TaskManager *taskManager) {
    if (MapObject_AreBitsSetForMovementScriptInit(work->mapObject) == TRUE) {
        MapObject_SetHeldMovement(work->mapObject, GetMovementForDirection(work->direction, 4));
        work->state++;
    }
    return 0;
}

int Whirlpool_CheckNextTile(WaterfallWork *work) {
    if (MapObject_IsMovementPaused(work->mapObject) == FALSE) {
        return 0;
    }
    MapObject_GetNextFacingDirection(work->mapObject);
    if (MetatileBehavior_IsWhirlpool(MapObject_GetCurrentMetatileBehavior(work->mapObject)) == TRUE) {
        work->state = 3;
        return 2;
    }
    WhirlpoolEffect_Delete(work->effect);
    work->effect = NULL;
    MapObject_ClearHeldMovementIfActive(work->mapObject);
    return 1;
}

void Field_PlayerAvatar_TryEnterPoketchPose(PlayerAvatar *playerAvatar) {
    u32 flags;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    if (MapObject_IsDrawInitialized(mapObject) != FALSE && PlayerAvatar_GetState(playerAvatar) == PLAYER_STATE_WALKING && MapObject_GetSpriteID(mapObject) - SPRITE_POKEHERO > 1) {
        flags = PlayerAvatar_GetTransitionFlags(playerAvatar);
        Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, PLAYER_TRANSITION_POKETCH);
        Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
        Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, flags);
    }
}

void Field_PlayerAvatar_TryLeavePoketchPose(PlayerAvatar *playerAvatar) {
    u32 flags;
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    if (MapObject_IsDrawInitialized(mapObject) != FALSE && PlayerAvatar_GetState(playerAvatar) == PLAYER_STATE_WALKING) {
        switch (MapObject_GetSpriteID(mapObject)) {
        case SPRITE_POKEHERO:
        case SPRITE_POKEHEROINE:
            flags = PlayerAvatar_GetTransitionFlags(playerAvatar);
            Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, PLAYER_TRANSITION_WALKING);
            Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
            Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, flags);
            break;
        }
    }
}

SysTask *Field_PlayerMovementSavingSet(FieldSystem *fieldSystem) {
    PlayerAvatar *playerAvatar = fieldSystem->playerAvatar;
    u32 state = PlayerAvatar_GetState(playerAvatar);
    PlayerSavingPoseWork *work;
    LocalMapObject *mapObject;
    SysTask *task;
    u32 flags;
    if (state != PLAYER_STATE_WALKING && state != PLAYER_STATE_ROCKET) {
        return NULL;
    }
    mapObject = PlayerAvatar_GetMapObject(playerAvatar);
    work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(PlayerSavingPoseWork));
    work->unk00 = 0;
    work->fieldSystem = fieldSystem;
    work->playerAvatar = playerAvatar;
    work->state = state;
    if (state == PLAYER_STATE_WALKING) {
        flags = PLAYER_TRANSITION_SAVING;
    } else if (state == PLAYER_STATE_ROCKET) {
        flags = PLAYER_TRANSITION_ROCKET_SAVING;
    } else {
        GF_AssertFail();
        flags = PLAYER_TRANSITION_SAVING;
    }
    MapObject_UnpauseMovement(mapObject);
    Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, flags);
    Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
    task = SysTask_CreateOnMainQueue(PlayerMovementSaving_Update, work, 0xFFFF);
    if (task == NULL) {
        GF_AssertFail();
    }
    return task;
}

void Field_PlayerMovementSavingClear(SysTask *task) {
    PlayerSavingPoseWork *work;
    int state;
    PlayerAvatar *playerAvatar;
    if (task == NULL) {
        return;
    }
    work = SysTask_GetData(task);
    state = work->state;
    playerAvatar = work->playerAvatar;
    if (state == PLAYER_STATE_WALKING) {
        Field_PlayerAvatar_TransitionToWalking(playerAvatar);
    } else if (state == PLAYER_STATE_ROCKET) {
        Field_PlayerAvatar_TransitionToRocket(playerAvatar);
    } else {
        GF_AssertFail();
        Field_PlayerAvatar_TransitionToWalking(playerAvatar);
    }
    Heap_Free(work);
    SysTask_Destroy(task);
}

void PlayerMovementSaving_Update(SysTask *task, void *data) {
    MapObject_CallDrawUpdate(PlayerAvatar_GetMapObject(((PlayerSavingPoseWork *)data)->playerAvatar));
}

void FieldMoveAnim_Init(FieldSystem *fieldSystem, Pokemon *mon, FieldMoveAnim *anim) {
    anim->useCutIn = TRUE;
    anim->mon = mon;
    anim->gender = PlayerAvatar_GetGender(fieldSystem->playerAvatar);
}

void FieldMoveAnim_Start(FieldSystem *fieldSystem, FieldMoveAnim *anim) {
    anim->cutIn = FieldMoveCutIn_Start(fieldSystem, 0, anim->mon, anim->gender);
}

BOOL FieldMoveAnim_CheckFinished(FieldMoveAnim *anim) {
    if (FieldMoveCutIn_IsFinished(anim->cutIn) == TRUE) {
        FieldMoveCutIn_Delete(anim->cutIn);
        return TRUE;
    }
    return FALSE;
}

void Field_PlayerAvatar_SetSprite(PlayerAvatar *playerAvatar, u32 sprite) {
    MapObject_ChangeBillboardSprite(PlayerAvatar_GetMapObject(playerAvatar), sprite);
}

LocalMapObject *Field_PlayerAvatar_GetFacingMapObject(PlayerAvatar *playerAvatar, int direction) {
    int x = PlayerAvatar_GetXCoord(playerAvatar);
    int dx = GetDeltaXByFacingDirection(direction);
    int z = PlayerAvatar_GetZCoord(playerAvatar);
    int dz = GetDeltaYByFacingDirection(direction);
    return MapObjectManager_GetFirstObjectWithXAndZ(MapObject_GetManager(PlayerAvatar_GetMapObject(playerAvatar)), x + dx, z + dz, FALSE);
}

void *FieldMoveTask_AllocWork(u32 size) {
    void *work = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
    if (work == NULL) {
        GF_AssertFail();
    }
    memset(work, 0, size);
    return work;
}

void FieldMoveTask_FreeWork(void *work) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, work);
}

Pokemon *Field_GetPartyMon(FieldSystem *fieldSystem, int partySlot) {
    return Party_GetMonByIndex(SaveArray_Party_Get(fieldSystem->saveData), partySlot);
}

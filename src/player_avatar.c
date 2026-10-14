#include "player_avatar.h"

#include "global.h"

#include "constants/global_fieldmap.h"
#include "constants/player_avatar.h"
#include "constants/sprites.h"

#include "assert.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_00_thumb.h"
#include "overlay_01.h"
#include "field_surf_mount.h"
#include "field_warp_arrow.h"
#include "player_data.h"
#include "scrcmd.h"
#include "communication_player_state.h"
#include "communication_core.h"
#include "map_object_movement_utilities.h"

static PlayerAvatar *PlayerAvatar_Create(void);
static void PlayerAvatar_Setup(PlayerAvatar *avatar, s32 state, u32 gender, PlayerSaveData *playerSaveData);
static void PlayerAvatar_CreateMapObjectWithParams(PlayerAvatar *avatar, MapObjectManager *manager, u32 sprite, u32 direction, u32 x, u32 z);
static LocalMapObject *PlayerAvatar_GetActiveMapObjectWithMovement1(MapObjectManager *manager);
static LocalMapObject *PlayerAvatar_GetMapObjectConst(PlayerAvatar *avatar);
static void PlayerAvatar_SetGender(PlayerAvatar *avatar, u32 gender);
static void PlayerAvatar_SetFlagsBits(PlayerAvatar *avatar, PlayerAvatarFlags flags);
static void PlayerAvatar_ClearFlagsBits(PlayerAvatar *avatar, PlayerAvatarFlags flags);
static PlayerAvatarFlags PlayerAvatar_GetFlagsBitsMask(PlayerAvatar *avatar, PlayerAvatarFlags flags);
static void PlayerAvatar_SetLastHorizontalInput(PlayerAvatar *avatar, s32 horizontalInput);
static void PlayerAvatar_SetLastVerticalInput(PlayerAvatar *avatar, s32 verticalInput);
static void PlayerAvatar_SetPlayerSaveData(PlayerAvatar *avatar, PlayerSaveData *playerSaveData);
static void PlayerSaveData_SetState(PlayerSaveData *playerSaveData, s32 state);
static void PlayerAvatar_SetPlayerSaveDataState(PlayerAvatar *avatar, s32 state);

u32 FrontierStat_GetFriendIndex(u32 stat) {
    if (stat < 100) {
        return 255;
    }
    return WifiConnection_GetTargetFriendIndex();
}

u8 Comm_GetPartnerGameVersion(void) {
    PlayerProfile *profile = CommPlayerInfo_GetProfile(Comm_GetLocalNetId() ^ 1);
    GF_ASSERT(profile != NULL);
    return PlayerProfile_GetVersion(profile);
}

u8 sub_0205C298(SaveData *saveData) {
    if (PlayerProfile_GetVersion(Save_PlayerData_GetProfile(saveData)) == 0) {
        return 1;
    }
    if (Comm_GetPartnerGameVersion() == 0) {
        return 1;
    }
    return 0;
}

u8 BattleArcade_GetWinStreakStat(u32 mode) {
    switch (mode) {
    case 0:
        return 95;
    case 1:
        return 97;
    case 2:
        return 99;
    case 3:
        return 143;
    }
}

u8 BattleArcade_GetBestStreakStat(u32 mode) {
    switch (mode) {
    case 0:
        return 94;
    case 1:
        return 96;
    case 2:
        return 98;
    case 3:
        return 142;
    }
}

u8 BattleTower_GetWinStreakStat(u32 mode) {
    u8 ret;
    switch (mode) {
    case 0:
        ret = 1;
        break;
    case 1:
        ret = 3;
        break;
    case 2:
        ret = 5;
        break;
    case 3:
        ret = 7;
        break;
    case 4:
        ret = 9;
        break;
    case 6:
        ret = 113;
        break;
    case 5:
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return ret;
}

u8 BattleTower_GetBestStreakStat(u32 mode) {
    u8 ret;
    switch (mode) {
    case 0:
        ret = 0;
        break;
    case 1:
        ret = 2;
        break;
    case 2:
        ret = 4;
        break;
    case 3:
        ret = 6;
        break;
    case 4:
        ret = 8;
        break;
    case 6:
        ret = 112;
        break;
    default:
    case 5:
        GF_ASSERT(FALSE);
        break;
    }
    return ret;
}

PlayerAvatar *PlayerAvatar_CreateWithParams(MapObjectManager *manager, u32 x, u32 z, u32 direction, s32 state, u32 gender, u32 param6, PlayerSaveData *playerSaveData) {
    PlayerAvatar *avatar = PlayerAvatar_Create();
    PlayerAvatar_Setup(avatar, state, gender, playerSaveData);
    u32 sprite;
    if (param6 == 2) {
        sprite = PlayerAvatar_GetSpriteByStateAndGender(state, gender);
    } else if (param6 == 0) {
        if (gender == PLAYER_GENDER_MALE) {
            sprite = SPRITE_PL_BOY01C;
        } else {
            sprite = SPRITE_PL_GIRL01C;
        }
    } else if (param6 == 1) {
        if (gender == PLAYER_GENDER_MALE) {
            sprite = SPRITE_HERO_2;
        } else {
            sprite = SPRITE_HEROINE_2;
        }
    } else {
        GF_ASSERT(FALSE);
        sprite = PlayerAvatar_GetSpriteByStateAndGender(state, gender);
    }
    PlayerAvatar_CreateMapObjectWithParams(avatar, manager, sprite, direction, x, z);
    return avatar;
}

PlayerAvatar *PlayerAvatar_CreateWithActiveMapObject(MapObjectManager *manager, PlayerSaveData *playerSaveData, u32 gender) {
    PlayerAvatar *avatar = PlayerAvatar_Create();
    s32 state = PlayerSaveData_GetState(playerSaveData);
    PlayerAvatar_Setup(avatar, state, gender, playerSaveData);
    LocalMapObject *mapObject = PlayerAvatar_GetActiveMapObjectWithMovement1(manager);
    MapObject_SetSpriteID(mapObject, PlayerAvatar_GetSpriteByStateAndGender(state, gender));
    MapObject_SetFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK13 | MAPOBJECTFLAG_KEEP));
    MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK8 | MAPOBJECTFLAG_UNK7));
    MapObject_SetFlag29(mapObject, TRUE);
    PlayerAvatar_SetMapObject(avatar, mapObject);
    return avatar;
}

void PlayerAvatar_InitMapFeatures(PlayerAvatar *avatar) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    GF_ASSERT(mapObject != NULL);
    MapObject_GetManager(mapObject);
    WarpArrow_Spawn(avatar);
    if (PlayerAvatar_GetState(avatar) == PLAYER_STATE_SURFING) {
        u32 x = PlayerAvatar_GetXCoord(avatar);
        u32 z = PlayerAvatar_GetZCoord(avatar);
        u32 direction = PlayerAvatar_GetFacingDirection(avatar);
        PlayerAvatar_SetSurfMountEffect(avatar, SurfMountEffect_Create(mapObject, x, z, direction, 1));
    }
}

void PlayerAvatar_FreeToHeap(PlayerAvatar *avatar) {
    Heap_Free(avatar);
}

void PlayerAvatar_DeleteFromMap(PlayerAvatar *avatar) {
    MapObject_Remove(PlayerAvatar_GetMapObject(avatar));
    PlayerAvatar_FreeToHeap(avatar);
}

static PlayerAvatar *PlayerAvatar_Create(void) {
    PlayerAvatar *avatar = Heap_Alloc(HEAP_ID_FIELD2, sizeof(PlayerAvatar));
    GF_ASSERT(avatar != NULL);
    memset(avatar, 0, sizeof(PlayerAvatar));
    return avatar;
}

static void PlayerAvatar_Setup(PlayerAvatar *avatar, s32 state, u32 gender, PlayerSaveData *playerSaveData) {
    PlayerAvatar_SetPlayerSaveData(avatar, playerSaveData);
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
    PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
    PlayerAvatar_SetState(avatar, state);
    PlayerAvatar_SetGender(avatar, gender);
    PlayerAvatar_SetTransitionFlags(avatar, 0);
    PlayerAvatar_ClearSpeedAndFlag2(avatar);
    PlayerAvatar_SetLastHorizontalInput(avatar, -1);
    PlayerAvatar_SetLastVerticalInput(avatar, -1);
    PlayerAvatar_SetHeldMovement(avatar, 255); // obj_player?
    PlayerAvatar_SetFlag1(avatar, TRUE);
}

static void PlayerAvatar_CreateMapObjectWithParams(PlayerAvatar *avatar, MapObjectManager *manager, u32 sprite, u32 direction, u32 x, u32 z) {
    LocalMapObject *mapObject = MapObject_Create(manager, x, z, direction, sprite, 1, 1);
    GF_ASSERT(mapObject != NULL);
    MapObject_SetID(mapObject, obj_player);
    MapObject_SetType(mapObject, 0);
    MapObject_SetEventFlag(mapObject, 0);
    MapObject_SetScriptID(mapObject, 0);
    MapObject_SetParam(mapObject, 0, 0);
    MapObject_SetParam(mapObject, 0, 1);
    MapObject_SetParam(mapObject, 0, 2);
    MapObject_SetXRange(mapObject, -1);
    MapObject_SetYRange(mapObject, -1);
    MapObject_SetFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK13 | MAPOBJECTFLAG_KEEP));
    MapObject_ClearFlagsBits(mapObject, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK8 | MAPOBJECTFLAG_UNK7));
    MapObject_SetFlag29(mapObject, TRUE);
    PlayerAvatar_SetMapObject(avatar, mapObject);
}

LocalMapObject *MapObjectManager_GetFirstActiveObjectWithMovement1(MapObjectManager *manager) {
    s32 index = 0;
    LocalMapObject *mapObject = NULL;
    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &mapObject, &index, MAPOBJECTFLAG_ACTIVE)) {
        if (MapObject_GetMovement(mapObject) == 1) {
            break;
        }
    }
    return mapObject;
}

static LocalMapObject *PlayerAvatar_GetActiveMapObjectWithMovement1(MapObjectManager *manager) {
    LocalMapObject *mapObject = MapObjectManager_GetFirstActiveObjectWithMovement1(manager);
    GF_ASSERT(mapObject != NULL);
    return mapObject;
}

u32 PlayerAvatar_GetFacingDirection(PlayerAvatar *avatar) {
    return MapObject_GetFacingDirection(PlayerAvatar_GetMapObject(avatar));
}

void PlayerAvatar_SetFacingDirection(PlayerAvatar *avatar, u32 direction) {
    MapObject_SetFacingDirection(PlayerAvatar_GetMapObject(avatar), direction);
}

u32 PlayerAvatar_GetNextFacingDirection(PlayerAvatar *avatar) {
    return MapObject_GetNextFacingDirection(PlayerAvatar_GetMapObject(avatar));
}

u32 PlayerAvatar_GetXCoord(PlayerAvatar *avatar) {
    return MapObject_GetXCoord(PlayerAvatar_GetMapObject(avatar));
}

u32 PlayerAvatar_GetZCoord(PlayerAvatar *avatar) {
    return MapObject_GetZCoord(PlayerAvatar_GetMapObject(avatar));
}

u32 PlayerAvatar_GetPreviousXCoord(PlayerAvatar *avatar) {
    return MapObject_GetPreviousXCoord(PlayerAvatar_GetMapObject(avatar));
}

u32 PlayerAvatar_GetPreviousZCoord(PlayerAvatar *avatar) {
    return MapObject_GetPreviousZCoord(PlayerAvatar_GetMapObject(avatar));
}

void PlayerAvatar_CopyPositionVector(PlayerAvatar *avatar, VecFx32 *vector) {
    MapObject_CopyPositionVector(PlayerAvatar_GetMapObject(avatar), vector);
}

VecFx32 *PlayerAvatar_GetPositionVector(PlayerAvatar *avatar) {
    return MapObject_GetPositionVector(PlayerAvatar_GetMapObjectConst(avatar));
}

void PlayerAvatar_SetMoveState(PlayerAvatar *avatar, u32 state) {
    avatar->moveState = state;
}

u32 PlayerAvatar_GetMoveState(PlayerAvatar *avatar) {
    return avatar->moveState;
}

void PlayerAvatar_SetPlayerMoveState(PlayerAvatar *avatar, u32 state) {
    avatar->playerMoveState = state;
}

u32 PlayerAvatar_GetPlayerMoveState(PlayerAvatar *avatar) {
    return avatar->playerMoveState;
}

void PlayerAvatar_SetMapObject(PlayerAvatar *avatar, LocalMapObject *mapObject) {
    avatar->mapObject = mapObject;
}

LocalMapObject *PlayerAvatar_GetMapObject(PlayerAvatar *avatar) {
    return avatar->mapObject;
}

static LocalMapObject *PlayerAvatar_GetMapObjectConst(PlayerAvatar *avatar) {
    return avatar->mapObject;
}

void PlayerAvatar_SetState(PlayerAvatar *avatar, s32 state) {
    GF_ASSERT(state < PLAYER_STATE_USE_HM);
    avatar->state = state;
    PlayerAvatar_SetPlayerSaveDataState(avatar, state);
}

s32 PlayerAvatar_GetState(PlayerAvatar *avatar) {
    GF_ASSERT(avatar != NULL);
    return avatar->state;
}

void PlayerAvatar_SetTransitionFlagsBits(PlayerAvatar *avatar, u32 flags) {
    avatar->transitionFlags |= flags;
}

void PlayerAvatar_SetTransitionFlags(PlayerAvatar *avatar, u32 flags) {
    avatar->transitionFlags = flags;
}

u32 PlayerAvatar_GetTransitionFlags(PlayerAvatar *avatar) {
    return avatar->transitionFlags;
}

static void PlayerAvatar_SetGender(PlayerAvatar *avatar, u32 gender) {
    avatar->gender = gender;
}

u32 PlayerAvatar_GetGender(PlayerAvatar *avatar) {
    return avatar->gender;
}

static void PlayerAvatar_SetFlagsBits(PlayerAvatar *avatar, PlayerAvatarFlags flags) {
    avatar->flags |= flags;
}

static void PlayerAvatar_ClearFlagsBits(PlayerAvatar *avatar, PlayerAvatarFlags flags) {
    avatar->flags &= ~flags;
}

static PlayerAvatarFlags PlayerAvatar_GetFlagsBitsMask(PlayerAvatar *avatar, PlayerAvatarFlags flags) {
    return (PlayerAvatarFlags)(avatar->flags & flags);
}

s32 PlayerAvatar_GetSpeed(PlayerAvatar *avatar) {
    return avatar->speed;
}

void PlayerAvatar_SetSpeed(PlayerAvatar *avatar, s32 speed) {
    avatar->speed = speed;
}

void PlayerAvatar_ClearSpeedAndFlag2(PlayerAvatar *avatar) {
    avatar->speed = 0;
    PlayerAvatar_SetFlag2(avatar, FALSE);
}

s32 PlayerAvatar_AddSpeedWithCeiling(PlayerAvatar *avatar, s32 delta, s32 ceiling) {
    s32 var = avatar->speed + delta;
    avatar->speed = var;
    if (var > ceiling) {
        avatar->speed = ceiling;
    }
    return avatar->speed;
}

static void PlayerAvatar_SetLastHorizontalInput(PlayerAvatar *avatar, s32 horizontalInput) {
    avatar->lastHorizontalInput = horizontalInput;
}

s32 PlayerAvatar_GetLastHorizontalInput(PlayerAvatar *avatar) {
    return avatar->lastHorizontalInput;
}

static void PlayerAvatar_SetLastVerticalInput(PlayerAvatar *avatar, s32 verticalInput) {
    avatar->lastVerticalInput = verticalInput;
}

s32 PlayerAvatar_GetLastVerticalInput(PlayerAvatar *avatar) {
    return avatar->lastVerticalInput;
}

void PlayerAvatar_SetLastInputs(PlayerAvatar *avatar, s32 unk28, s32 unk2C) {
    PlayerAvatar_SetLastHorizontalInput(avatar, unk28);
    PlayerAvatar_SetLastVerticalInput(avatar, unk2C);
}

void PlayerAvatar_SetSurfMountEffect(PlayerAvatar *avatar, u32 surfMountEffect) {
    avatar->surfMountEffect = surfMountEffect;
}

u32 PlayerAvatar_GetSurfMountEffect(PlayerAvatar *avatar) {
    return avatar->surfMountEffect;
}

static void PlayerAvatar_SetPlayerSaveData(PlayerAvatar *avatar, PlayerSaveData *playerSaveData) {
    avatar->playerSaveData = playerSaveData;
}

PlayerSaveData *PlayerAvatar_GetPlayerSaveData(PlayerAvatar *avatar) {
    return avatar->playerSaveData;
}

void PlayerAvatar_SetHeldMovement(PlayerAvatar *avatar, u32 heldMovement) {
    avatar->heldMovement = heldMovement;
}

u32 PlayerAvatar_GetHeldMovement(PlayerAvatar *avatar) {
    return avatar->heldMovement;
}

BOOL PlayerAvatar_GetFootstepToggle(PlayerAvatar *avatar) {
    return avatar->footstepToggle;
}

void PlayerAvatar_ToggleFootstep(PlayerAvatar *avatar) {
    avatar->footstepToggle = (avatar->footstepToggle + 1) & TRUE;
}

void PlayerAvatar_ResetFootstepToggle(PlayerAvatar *avatar) {
    avatar->footstepToggle = FALSE;
}

void PlayerSaveData_Init(PlayerSaveData *playerSaveData) {
    playerSaveData->hasRunningShoes = FALSE;
    playerSaveData->runningShoesLock = FALSE;
    playerSaveData->state = 0;
}

BOOL PlayerSaveData_CheckRunningShoes(PlayerSaveData *playerSaveData) {
    if (playerSaveData != NULL && playerSaveData->hasRunningShoes == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void PlayerSaveData_SetRunningShoesFlag(PlayerSaveData *playerSaveData, BOOL flag) {
    if (flag == TRUE) {
        playerSaveData->hasRunningShoes = TRUE;
    } else {
        playerSaveData->hasRunningShoes = FALSE;
    }
}

s32 PlayerSaveData_GetState(PlayerSaveData *playerSaveData) {
    if (!playerSaveData) {
        return 0;
    }
    return playerSaveData->state;
}

static void PlayerSaveData_SetState(PlayerSaveData *playerSaveData, s32 state) {
    if (!playerSaveData) {
        return;
    }
    playerSaveData->state = state;
}

static void PlayerAvatar_SetPlayerSaveDataState(PlayerAvatar *avatar, s32 state) {
    PlayerSaveData_SetState(PlayerAvatar_GetPlayerSaveData(avatar), state);
}

void PlayerAvatar_SetPositionAndDirection(PlayerAvatar *avatar, VecFx32 *position, u32 direction) {
    MapObject_SetPositionFromVectorAndDirection(PlayerAvatar_GetMapObject(avatar), position, direction);
    PlayerAvatar_SetMoveState(avatar, AVATAR_MOVE_STATE_NONE);
    PlayerAvatar_SetPlayerMoveState(avatar, PLAYER_MOVE_STATE_NONE);
}

void PlayerAvatar_SetMapObjectYPosition(PlayerAvatar *avatar, fx32 yVal) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    VecFx32 position;
    MapObject_CopyPositionVector(mapObject, &position);
    position.y = yVal;
    MapObject_SetPositionVector(mapObject, &position);
}

void PlayerAvatar_ToggleAutomaticHeightUpdating(PlayerAvatar *avatar, u8 flag) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    if (flag == TRUE) {
        MapObject_SetIgnoreHeights(mapObject, FALSE);
    } else {
        MapObject_SetIgnoreHeights(mapObject, TRUE);
    }
}

void PlayerAvatar_ToggleAutomaticHeightUpdatingImmediate(PlayerAvatar *avatar, BOOL flag) {
    LocalMapObject *mapObject = PlayerAvatar_GetMapObject(avatar);
    if (flag == TRUE) {
        MapObject_SetIgnoreHeights(mapObject, FALSE);
        MapObject_UpdateHeight(mapObject);
    } else {
        MapObject_SetIgnoreHeights(mapObject, TRUE);
    }
}

u32 PlayerAvatar_GetSpriteByStateAndGender(s32 state, u32 gender) {
    if (gender == PLAYER_GENDER_MALE) {
        switch (state) {
        case PLAYER_STATE_WALKING:
            return SPRITE_HERO;
        case PLAYER_STATE_CYCLING:
            return SPRITE_CYCLEHERO;
        case PLAYER_STATE_SURFING:
            return SPRITE_SWIMHERO;
        case PLAYER_STATE_USE_HM:
            return SPRITE_SPHERO;
        case PLAYER_STATE_WATERING:
            return SPRITE_WATERHERO;
        case PLAYER_STATE_FISHING:
            return SPRITE_FISHINGHERO;
        case PLAYER_STATE_POKETCH:
            return SPRITE_POKEHERO;
        case PLAYER_STATE_SAVING:
            return SPRITE_SAVEHERO;
        case PLAYER_STATE_HEAL:
            return SPRITE_BANZAIHERO;
        case PLAYER_STATE_LADDER:
            return SPRITE_LADDERHERO;
        case PLAYER_STATE_ROCKET:
            return SPRITE_RHERO;
        case PLAYER_STATE_ROCKET_HEAL:
            return SPRITE_RBANZAIHERO;
        case PLAYER_STATE_POKEATHLON:
            return SPRITE_PKTHHERO;
        case PLAYER_STATE_APRICORN_SHAKE:
            return SPRITE_SHAKEHERO;
        case PLAYER_STATE_ROCKET_SAVING:
            return SPRITE_SAVERHERO;
        }
    } else {
        switch (state) {
        case PLAYER_STATE_WALKING:
            return SPRITE_HEROINE;
        case PLAYER_STATE_CYCLING:
            return SPRITE_CYCLEHEROINE;
        case PLAYER_STATE_SURFING:
            return SPRITE_SWIMHEROINE;
        case PLAYER_STATE_USE_HM:
            return SPRITE_SPHEROINE;
        case PLAYER_STATE_WATERING:
            return SPRITE_WATERHEROINE;
        case PLAYER_STATE_FISHING:
            return SPRITE_FISH_HEROINE;
        case PLAYER_STATE_POKETCH:
            return SPRITE_POKEHEROINE;
        case PLAYER_STATE_SAVING:
            return SPRITE_SAVEHEROINE;
        case PLAYER_STATE_HEAL:
            return SPRITE_BANZAIHEROINE;
        case PLAYER_STATE_LADDER:
            return SPRITE_LADDERHEROINE;
        case PLAYER_STATE_ROCKET:
            return SPRITE_RHEROINE;
        case PLAYER_STATE_ROCKET_HEAL:
            return SPRITE_RBANZAIINE;
        case PLAYER_STATE_POKEATHLON:
            return SPRITE_PKTHHEROINE;
        case PLAYER_STATE_APRICORN_SHAKE:
            return SPRITE_SHAKEHEROINE;
        case PLAYER_STATE_ROCKET_SAVING:
            return SPRITE_SAVERHEROINE;
        }
    }
    GF_ASSERT(FALSE);
    return 0;
}

u32 PlayerAvatar_GetTransitionBits(s32 state) {
    switch (state) {
    case PLAYER_STATE_WALKING:
        return PLAYER_TRANSITION_WALKING;
    case PLAYER_STATE_CYCLING:
        return PLAYER_TRANSITION_CYCLING;
    case PLAYER_STATE_SURFING:
        return PLAYER_TRANSITION_SURFING;
    case PLAYER_STATE_WATERING:
        return PLAYER_TRANSITION_WATERING;
    case PLAYER_STATE_FISHING:
        return PLAYER_TRANSITION_FISHING;
    case PLAYER_STATE_POKETCH:
        return PLAYER_TRANSITION_POKETCH;
    case PLAYER_STATE_SAVING:
        return PLAYER_TRANSITION_SAVING;
    case PLAYER_STATE_HEAL:
        return PLAYER_TRANSITION_HEAL;
    case PLAYER_STATE_LADDER:
        return PLAYER_TRANSITION_LADDER;
    case PLAYER_STATE_ROCKET:
        return PLAYER_TRANSITION_ROCKET;
    case PLAYER_STATE_ROCKET_HEAL:
        return PLAYER_TRANSITION_ROCKET_HEAL;
    case PLAYER_STATE_POKEATHLON:
        return PLAYER_TRANSITION_POKEATHLON;
    case PLAYER_STATE_APRICORN_SHAKE:
        return PLAYER_TRANSITION_APRICORN_SHAKE;
    case PLAYER_STATE_ROCKET_SAVING:
        return PLAYER_TRANSITION_ROCKET_SAVING;
    case PLAYER_STATE_USE_HM:
    default:
        GF_ASSERT(FALSE);
        return PLAYER_TRANSITION_WALKING;
    }
}

PlayerAvatar *FieldSystem_GetPlayerAvatar(FieldSystem *fieldSystem) {
    return fieldSystem->playerAvatar;
}

void PlayerAvatar_SetForcedMovement(PlayerAvatar *avatar, BOOL set) {
    if (set == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_FORCED_MOVEMENT);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_FORCED_MOVEMENT);
    }
}

BOOL PlayerAvatar_CheckForcedMovement(PlayerAvatar *avatar) {
    if (PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_FORCED_MOVEMENT)) {
        return TRUE;
    }
    return FALSE;
}

void PlayerAvatar_SetFlag1(PlayerAvatar *avatar, BOOL set) {
    if (set == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_UNK1);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_UNK1);
    }
}

BOOL PlayerAvatar_CheckFlag1(PlayerAvatar *avatar) {
    if (PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_UNK1)) {
        return TRUE;
    }
    return FALSE;
}

void PlayerAvatar_SetFlag2(PlayerAvatar *avatar, BOOL set) {
    if (set == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_UNK2);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_UNK2);
    }
}

BOOL PlayerAvatar_CheckFlag2(PlayerAvatar *avatar) {
    if (PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_UNK2)) {
        return TRUE;
    }
    return FALSE;
}

void PlayerAvatar_SetBikeStateLock(PlayerAvatar *avatar, BOOL lock) {
    if (lock == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_LOCK_BIKE_STATE);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_LOCK_BIKE_STATE);
    }
}

BOOL PlayerAvatar_CheckBikeStateLocked(PlayerAvatar *avatar) {
    if (PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_LOCK_BIKE_STATE)) {
        return TRUE;
    }
    return FALSE;
}

void PlayerAvatar_SetFlag5(PlayerAvatar *avatar, BOOL set) {
    if (set == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_UNK5);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_UNK5);
    }
}

void PlayerAvatar_SetFlag6(PlayerAvatar *avatar) {
    PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_UNK6);
}

void PlayerAvatar_ClearFlag6(PlayerAvatar *avatar) {
    PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_UNK6);
}

BOOL PlayerAvatar_CheckFlag6(PlayerAvatar *avatar) {
    if (PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_UNK6)) {
        return TRUE;
    }
    return FALSE;
}

void PlayerAvatar_SetFlag7(PlayerAvatar *avatar, BOOL set) {
    if (set == TRUE) {
        PlayerAvatar_SetFlagsBits(avatar, AVATAR_FLAG_UNK7);
    } else {
        PlayerAvatar_ClearFlagsBits(avatar, AVATAR_FLAG_UNK7);
    }
}

BOOL PlayerAvatar_CheckFlag7(PlayerAvatar *avatar) {
    return PlayerAvatar_GetFlagsBitsMask(avatar, AVATAR_FLAG_UNK7);
}

u16 PlayerAvatar_CheckRunningShoesLock(PlayerAvatar *avatar) {
    return avatar->playerSaveData->runningShoesLock;
}

void PlayerAvatar_SetRunningShoesLock(PlayerAvatar *avatar, u16 lock) {
    avatar->playerSaveData->runningShoesLock = lock;
}

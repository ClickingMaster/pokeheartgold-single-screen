#ifndef POKEHEARTGOLD_PLAYER_AVATAR_H
#define POKEHEARTGOLD_PLAYER_AVATAR_H

#include "global.h"

#include "field_types_def.h"
#include "save.h"

typedef enum PlayerAvatarFlags {
    AVATAR_FLAG_FORCED_MOVEMENT = (1 << 0),
    AVATAR_FLAG_UNK1 = (1 << 1),
    AVATAR_FLAG_UNK2 = (1 << 2),
    AVATAR_FLAG_LOCK_BIKE_STATE = (1 << 3), // Blocks the player from mounting/dismounting their bike (though this was intended to stop dismounting during Cycling Road)
    AVATAR_FLAG_UNK4 = (1 << 4),
    AVATAR_FLAG_UNK5 = (1 << 5),
    AVATAR_FLAG_UNK6 = (1 << 6),
    AVATAR_FLAG_UNK7 = (1 << 7),
} PlayerAvatarFlags;

typedef struct PlayerSaveData {
    u16 hasRunningShoes; // bool16
    u16 runningShoesLock;
    s32 state;
} PlayerSaveData;

struct PlayerAvatar {
    PlayerAvatarFlags flags;
    u32 transitionFlags;
    u32 heldMovement;
    BOOL footstepToggle;
    u32 moveState;
    u32 playerMoveState;
    s32 state;
    u32 gender;
    u32 unk20;
    s32 speed;
    s32 lastHorizontalInput;
    s32 lastVerticalInput;
    LocalMapObject *mapObject;
    u32 surfMountEffect;
    PlayerSaveData *playerSaveData;
    u32 unk3c;
}; // size: 0x40

u32 FrontierStat_GetFriendIndex(u32 param0);
u8 Comm_GetPartnerGameVersion(void);
u8 sub_0205C298(SaveData *saveData);
u8 BattleArcade_GetWinStreakStat(u32 param0);
u8 BattleArcade_GetBestStreakStat(u32 param0);
u8 BattleTower_GetWinStreakStat(u32 param0);
u8 BattleTower_GetBestStreakStat(u32 param0);
PlayerAvatar *PlayerAvatar_CreateWithParams(MapObjectManager *manager, u32 x, u32 z, u32 direction, s32 state, u32 gender, u32 param6, PlayerSaveData *playerSaveData);
PlayerAvatar *PlayerAvatar_CreateWithActiveMapObject(MapObjectManager *manager, PlayerSaveData *playerSaveData, u32 gender);
void PlayerAvatar_InitMapFeatures(PlayerAvatar *avatar);
void PlayerAvatar_FreeToHeap(PlayerAvatar *avatar);
void PlayerAvatar_DeleteFromMap(PlayerAvatar *avatar);
LocalMapObject *MapObjectManager_GetFirstActiveObjectWithMovement1(MapObjectManager *manager);
u32 PlayerAvatar_GetFacingDirection(PlayerAvatar *avatar);
void PlayerAvatar_SetFacingDirection(PlayerAvatar *avatar, u32 direction);
u32 PlayerAvatar_GetNextFacingDirection(PlayerAvatar *avatar);
u32 PlayerAvatar_GetXCoord(PlayerAvatar *avatar);
u32 PlayerAvatar_GetZCoord(PlayerAvatar *avatar);
u32 PlayerAvatar_GetPreviousXCoord(PlayerAvatar *avatar);
u32 PlayerAvatar_GetPreviousZCoord(PlayerAvatar *avatar);
void PlayerAvatar_CopyPositionVector(PlayerAvatar *avatar, VecFx32 *vector);
VecFx32 *PlayerAvatar_GetPositionVector(PlayerAvatar *avatar);
void PlayerAvatar_SetMoveState(PlayerAvatar *avatar, u32 state);
u32 PlayerAvatar_GetMoveState(PlayerAvatar *avatar);
void PlayerAvatar_SetPlayerMoveState(PlayerAvatar *avatar, u32 param1);
u32 PlayerAvatar_GetPlayerMoveState(PlayerAvatar *avatar);
void PlayerAvatar_SetMapObject(PlayerAvatar *avatar, LocalMapObject *mapObject);
LocalMapObject *PlayerAvatar_GetMapObject(PlayerAvatar *avatar);
void PlayerAvatar_SetState(PlayerAvatar *avatar, s32 state);
s32 PlayerAvatar_GetState(PlayerAvatar *avatar);
void PlayerAvatar_SetTransitionFlagsBits(PlayerAvatar *avatar, u32 flags);
void PlayerAvatar_SetTransitionFlags(PlayerAvatar *avatar, u32 flags);
u32 PlayerAvatar_GetTransitionFlags(PlayerAvatar *avatar);
u32 PlayerAvatar_GetGender(PlayerAvatar *avatar);
s32 PlayerAvatar_GetUnk24(PlayerAvatar *avatar);
void PlayerAvatar_SetUnk24(PlayerAvatar *avatar, s32 param1);
void PlayerAvatar_ClearUnk24ClearFlag2(PlayerAvatar *avatar);
s32 PlayerAvatar_Unk24AddWithCeiling(PlayerAvatar *avatar, s32 param1, s32 param2);
s32 PlayerAvatar_GetUnk28(PlayerAvatar *avatar);
s32 PlayerAvatar_GetUnk2C(PlayerAvatar *avatar);
void PlayerAvatar_SetUnk28Unk2C(PlayerAvatar *avatar, s32 unk28, s32 unk2C);
void PlayerAvatar_SetUnk34(PlayerAvatar *avatar, u32 param1);
u32 PlayerAvatar_GetUnk34(PlayerAvatar *avatar);
PlayerSaveData *PlayerAvatar_GetPlayerSaveData(PlayerAvatar *avatar);
void PlayerAvatar_SetUnk8(PlayerAvatar *avatar, u32 unkA);
u32 PlayerAvatar_GetUnk8(PlayerAvatar *avatar);
BOOL PlayerAvatar_GetUnkC(PlayerAvatar *avatar);
void PlayerAvatar_ToggleUnkC(PlayerAvatar *avatar);
void PlayerAvatar_ResetUnkC(PlayerAvatar *avatar);
void PlayerSaveData_Init(PlayerSaveData *playerSaveData);
BOOL PlayerSaveData_CheckRunningShoes(PlayerSaveData *playerSaveData);
void PlayerSaveData_SetRunningShoesFlag(PlayerSaveData *playerSaveData, BOOL flag);
s32 PlayerSaveData_GetState(PlayerSaveData *playerSaveData);
void PlayerAvatar_SetPositionAndDirection(PlayerAvatar *avatar, VecFx32 *position, u32 direction);
void PlayerAvatar_SetMapObjectYPosition(PlayerAvatar *avatar, fx32 yVal);
void PlayerAvatar_ToggleAutomaticHeightUpdating(PlayerAvatar *avatar, u8 flag);
void PlayerAvatar_ToggleAutomaticHeightUpdatingImmediate(PlayerAvatar *avatar, BOOL flag);
u32 PlayerAvatar_GetSpriteByStateAndGender(s32 state, u32 gender);
u32 PlayerAvatar_GetTransitionBits(s32 state);
PlayerAvatar *FieldSystem_GetPlayerAvatar(FieldSystem *fieldSystem);
void PlayerAvatar_SetForcedMovement(PlayerAvatar *avatar, BOOL set);
BOOL PlayerAvatar_CheckForcedMovement(PlayerAvatar *avatar);
void PlayerAvatar_SetFlag1(PlayerAvatar *avatar, BOOL set);
BOOL PlayerAvatar_CheckFlag1(PlayerAvatar *avatar);
void PlayerAvatar_SetFlag2(PlayerAvatar *avatar, BOOL set);
BOOL PlayerAvatar_CheckFlag2(PlayerAvatar *avatar);
void PlayerAvatar_SetBikeStateLock(PlayerAvatar *avatar, BOOL lock);
BOOL PlayerAvatar_CheckBikeStateLocked(PlayerAvatar *avatar);
void PlayerAvatar_SetFlag5(PlayerAvatar *avatar, BOOL set);
void PlayerAvatar_SetFlag6(PlayerAvatar *avatar);
void PlayerAvatar_ClearFlag6(PlayerAvatar *avatar);
BOOL PlayerAvatar_CheckFlag6(PlayerAvatar *avatar);
void PlayerAvatar_SetFlag7(PlayerAvatar *avatar, BOOL set);
BOOL PlayerAvatar_CheckFlag7(PlayerAvatar *avatar);
u16 PlayerAvatar_CheckRunningShoesLock(PlayerAvatar *avatar);
void PlayerAvatar_SetRunningShoesLock(PlayerAvatar *avatar, u16 lock);

#endif // POKEHEARTGOLD_PLAYER_AVATAR_H

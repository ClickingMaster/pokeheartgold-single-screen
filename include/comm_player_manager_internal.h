#ifndef POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H

#include "global.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "sys_task_api.h"

// Communication player manager: the avatars of the linked players in a shared field map. The host (net id 0) moves
// every player and sends the positions; the other players walk their avatars to the positions they receive.

typedef struct CommPlayerPosition CommPlayerPosition;
typedef struct CommPlayerManager CommPlayerManager;

// A player's position on the map. hostPositions: where the host (net id 0) has placed each player;
// positions: the positions received from the host, which the avatars walk to.
struct CommPlayerPosition {
    u16 x;
    u16 z;
    s8 facing;
    u8 speed;
    u8 unk_6;
    u8 bumped; // the last step was refused (target occupied)
};

struct CommPlayerManager {
    u8 unk_00[4];
    PlayerAvatar *avatars[8];
    u8 active[8];
    SysTask *task;
    FieldSystem *fieldSystem;
    CommPlayerPosition hostPositions[8];
    CommPlayerPosition positions[8];
    u8 moveEnabled[8];
    u8 inputLocked[8];
    u8 unk_C4[8];
    u8 positionChanged[8]; // the host must send this player's position
    u8 hostMoveTimer[8];
    u8 moveTimer[8];
    u8 unk_E4[2];
    u8 lockOnSpot[5];
    u8 localPositionSent;
    u8 sendAllPositions;
    u8 localMoveEnabled; // sent with command 0x3E
    u8 unk_EE;
    u8 fieldResumed;
    u8 localMoveDelay;
    u8 localWalkEnabled;
    u8 unk_F2[2];
};

extern CommPlayerManager *gCommPlayerManager;
extern const u16 _020FC7A0[4];
extern const u16 _020FC7A8[8];

BOOL CommPlayerManager_Init(CommPlayerManager *manager, FieldSystem *fieldSystem, int unused);
void CommPlayerManager_RemoveAllAvatars(void);
void CommPlayerManager_ResyncLocalPlayer(void);
void CommPlayerManager_Delete(int deleteFromMap);
BOOL CommPlayerManager_IsInitialized(void);
void CommPlayerManager_InitLocalPlayer(void);
void CommPlayerManager_SendPosition(int forceSend, int x, int z);
void CommPlayerManager_SendLocalPosition(int forceSend);
void CommPlayerManager_SendHostPosition(int netId, const CommPlayerPosition *position);
void CommPlayerManager_SendPositionWhenStopped(FieldSystem *unused);
void CommPlayerManager_SendChangedHostPositions(FieldSystem *unused);
u32 CommPlayerManager_sizeof(void);
void CommPlayerManager_CreateAvatar(int netId);
void CommPlayerManager_RemovePlayer(int netId, int keepActive, int deleteFromMap);
void CommPlayerManager_UpdateLocalMoveSpeed(FieldSystem *fieldSystem);
void CommPlayerManager_Task(SysTask *task, void *fieldSystem);
void CommPlayerManager_UpdateAvatars(FieldSystem *unused);
void CommPlayerManager_SendMoveEnabled(int value);
void CommPlayerManager_SetMoveEnabled(int value);
void CommPlayerManager_ReceiveMoveEnabled(int netId, int size, const u8 *data);
void CommPlayerManager_FieldUpdate(FieldSystem *fieldSystem, int value);
int CommPlayerManager_KeysToDirection(int unused, int keys);
BOOL CommPlayerManager_IsObjectEventAt(int x, int z);
BOOL CommPlayerManager_IsPositionBlocked(int x, int z, int excludedNetId);
u32 CommPlayerManager_GetMoveDelay(int speed);
void CommPlayerManager_MoveHostPlayers(SysTask *task, CommPlayerManager *unused);
void CommPlayerManager_ReceivePosition(int netId, int size, const u8 *data);
void CommPlayerManager_ReceiveRemovePlayer(int unused, int size, const u8 *data, FieldSystem *fieldSystem);
int CommPlayerManager_GetPositionSize(void);
void CommPlayerManager_ReceiveHostPosition(int unused, int size, const u8 *data);
void CommPlayerManager_MoveAvatar(int netId);
int CommPlayerManager_GetHostPositionSize(void);
BOOL CommPlayerManager_IsLocalPlayerConnected(void);
int CommPlayerManager_GetPlayerX(int netId);
int CommPlayerManager_GetPlayerZ(int netId);
int CommPlayerManager_GetPlayerFrontX(int netId);
int CommPlayerManager_GetPlayerFrontZ(int netId);
int CommPlayerManager_GetHostX(int netId);
int CommPlayerManager_GetHostZ(int netId);
int CommPlayerManager_GetHostFrontX(int netId);
int CommPlayerManager_GetHostFrontZ(int netId);
void CommPlayerManager_SetPlayerMoveEnabled(int netId, int value);
BOOL CommPlayerManager_AssignBattleSlots(void);
void CommPlayerManager_RecvUnkE6(int netId, int size, const u8 *data);
void CommPlayerManager_HostLockFlaggedPlayersOnSpots(void);
void CommPlayerManager_FaceRoomCenter(void);
BOOL CommPlayerManager_StepAllPlayersBack(void);
int CommPlayerManager_GetOppositeDirection(int facing);
int CommPlayerManager_GetMoveTimer(int netId);
void CommPlayerManager_SetLocalFacing(int facing);
void CommPlayerManager_PauseField(void);
void CommPlayerManager_ResumeField(void);
void CommPlayerManager_ResumeFieldOnce(void);
void CommPlayerManager_ForcePosition(void);
void CommPlayerManager_SetLocalMoveDelay(void);

#endif // POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H

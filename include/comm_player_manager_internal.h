#ifndef POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H

#include "global.h"

#include "field_types_def.h"
#include "player_avatar.h"
#include "sys_task_api.h"

// Communication player manager (ARM9 static 0x02056D7C-0x02058034): the avatars of the linked players in a
// shared field map. The host (net id 0) moves every player by its key input and sends the positions; the other
// players walk their avatars to the positions they receive. The module's full declarations; include/unk_02056D7C.h
// keeps the declarations other modules were matched against. comm_player_manager.c: the manager and its set-up;
// comm_player_sync.c: the rest (one TU in the original; the lobby tables must stay outside the sync TU, whose
// sub_02057C94 was matched against them as external symbols).

typedef struct CommPlayerPosition CommPlayerPosition;
typedef struct CommPlayerManager CommPlayerManager;

// A player's position on the map. hostPositions: where the host (net id 0) has placed each player;
// positions: the positions received from the host, which the avatars walk to.
struct CommPlayerPosition {
    u16 x;     // 0x0
    u16 z;     // 0x2
    s8 facing; // 0x4
    u8 speed;  // 0x5
    u8 unk_6;  // 0x6
    u8 bumped; // 0x7: the last step was refused (target occupied)
};

struct CommPlayerManager {
    u8 unk_00[4];                        // 0x00
    PlayerAvatar *avatars[8];            // 0x04
    u8 active[8];                        // 0x24
    SysTask *task;                       // 0x2C
    FieldSystem *fieldSystem;            // 0x30
    CommPlayerPosition hostPositions[8]; // 0x34
    CommPlayerPosition positions[8];     // 0x74
    u8 unk_B4[8];                        // 0xB4
    u8 unk_BC[8];                        // 0xBC
    u8 unk_C4[8];                        // 0xC4
    u8 positionChanged[8];               // 0xCC: the host must send this player's position
    u8 hostMoveTimer[8];                 // 0xD4
    u8 moveTimer[8];                     // 0xDC
    u8 unk_E4[2];                        // 0xE4
    u8 unk_E6[5];                        // 0xE6
    u8 localPositionSent;                // 0xEB
    u8 sendAllPositions;                 // 0xEC
    u8 unk_ED;                           // 0xED: sent with command 0x3E
    u8 unk_EE;                           // 0xEE
    u8 unk_EF;                           // 0xEF
    u8 localMoveDelay;                   // 0xF0
    u8 unk_F1;                           // 0xF1
    u8 unk_F2[2];                        // 0xF2
};

extern CommPlayerManager *gCommPlayerManager;
extern const u16 _020FC7A0[4];
extern const u16 _020FC7A8[8];
extern const u16 _0210FAB4[4];
extern const u16 _0210FABC[4];
extern const u16 _0210FAC4[4];

BOOL CommPlayerManager_Init(CommPlayerManager *manager, FieldSystem *fieldSystem, int unused);
void CommPlayerManager_RemoveAllAvatars(void);
void sub_02056E60(void);
void CommPlayerManager_Delete(int deleteFromMap);
BOOL CommPlayerManager_IsInitialized(void);
void CommPlayerManager_InitLocalPlayer(void);
void CommPlayerManager_SendPosition(int forceSend, int x, int z);
void CommPlayerManager_SendLocalPosition(int forceSend);
void CommPlayerManager_SendHostPosition(int netId, const CommPlayerPosition *position);
void sub_020570C8(FieldSystem *unused);
void CommPlayerManager_SendChangedHostPositions(FieldSystem *unused);
u32 CommPlayerManager_sizeof(void);
void CommPlayerManager_CreateAvatar(int netId);
void CommPlayerManager_RemovePlayer(int netId, int keepActive, int deleteFromMap);
void sub_020572AC(FieldSystem *fieldSystem);
void CommPlayerManager_Task(SysTask *task, void *fieldSystem);
void CommPlayerManager_UpdateAvatars(FieldSystem *unused);
void sub_02057358(int value);
void sub_02057384(int value);
void sub_020573AC(int netId, int size, const u8 *data);
void sub_020573F0(FieldSystem *fieldSystem, int value);
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
void sub_02057C5C(int netId, int value);
BOOL sub_02057C94(void);
void sub_02057D50(int netId, int size, const u8 *data);
void sub_02057D74(void);
void sub_02057E08(void);
BOOL sub_02057E50(void);
int CommPlayerManager_GetOppositeDirection(int facing);
int CommPlayerManager_GetMoveTimer(int netId);
void CommPlayerManager_SetLocalFacing(int facing);
void sub_02057F58(void);
void sub_02057F70(void);
void sub_02057F80(void);
void CommPlayerManager_ForcePosition(void);
void CommPlayerManager_SetLocalMoveDelay(void);

#endif // POKEHEARTGOLD_COMM_PLAYER_MANAGER_INTERNAL_H

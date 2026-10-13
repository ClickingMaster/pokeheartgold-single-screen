#ifndef POKEHEARTGOLD_COMM_PLAYER_MANAGER_H
#define POKEHEARTGOLD_COMM_PLAYER_MANAGER_H

#include "script.h"

void CommPlayerManager_RemoveAllAvatars(void);
void CommPlayerManager_FieldUpdate(FieldSystem *fieldSystem, int a1);
int CommPlayerManager_IsLocalPlayerConnected(void);
void CommPlayerManager_SetLocalFacing(u16 val);
void CommPlayerManager_ReceivePosition(void);
u32 CommPlayerManager_GetPositionSize(void);
void CommPlayerManager_ReceiveHostPosition(void);
void CommPlayerManager_ReceiveRemovePlayer(void);
void CommPlayerManager_ReceiveMoveEnabled(void);
void sub_02057D50(void);
void CommPlayerManager_ForcePosition();

#endif // POKEHEARTGOLD_COMM_PLAYER_MANAGER_H

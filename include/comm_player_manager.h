#ifndef POKEHEARTGOLD_COMM_PLAYER_MANAGER_H
#define POKEHEARTGOLD_COMM_PLAYER_MANAGER_H

#include "script.h"

void CommPlayerManager_RemoveAllAvatars(void);
void CommPlayerManager_FieldUpdate(FieldSystem *fieldSystem, int value);
int CommPlayerManager_IsLocalPlayerConnected(void);
void CommPlayerManager_SetLocalFacing(u16 val);
void CommPlayerManager_ReceivePosition(void);
u32 CommPlayerManager_GetPositionSize(void);
void CommPlayerManager_ReceiveHostPosition(void);
void CommPlayerManager_ReceiveRemovePlayer(void);
void CommPlayerManager_ReceiveMoveEnabled(void);
void CommPlayerManager_RecvUnkE6(void);
void CommPlayerManager_ForcePosition();

#endif // POKEHEARTGOLD_COMM_PLAYER_MANAGER_H

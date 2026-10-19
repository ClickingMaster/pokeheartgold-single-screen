#ifndef POKEHEARTGOLD_COMMUNICATION_CORE_H
#define POKEHEARTGOLD_COMMUNICATION_CORE_H

#include "global.h"

#include "safari_zone.h"

BOOL Comm_Update(void);
BOOL Comm_IsNetIdConnected(u16 slot);
BOOL Comm_SendCommand(int command, void *data, int size);
int Comm_CountConnected(void);
BOOL Comm_IsActive(void);
void Comm_EnableKeySync(void);
void Comm_DisableKeySync(void);
int Comm_GetLocalNetId(void);
BOOL Comm_SendFixedCommandNoCopy(int command, void *data);
BOOL Comm_SendEmptyCommand(int command);
BOOL Comm_HasLinkError(void);
int Comm_GetMaxPlayers(int value);
int Comm_GetMinPlayers(int value);
int Comm_GetBattleSlot(int slot);
void CommState_SetUnionAppLinkMode(void);
void CommState_SetUnionRoomLinkMode(void);
BOOL Comm_ParentQueueHasCommand(int value);
BOOL Comm_IsWifiConnected(void);
BOOL Comm_IsSendingMoveKeys(void);

#endif // POKEHEARTGOLD_COMMUNICATION_CORE_H

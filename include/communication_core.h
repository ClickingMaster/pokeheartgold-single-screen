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
BOOL Comm_GetLocalNetId(void);
void Comm_SendFixedCommandNoCopy(s32 command, SafariZoneAreaSet *areaSet);
void Comm_SendEmptyCommand(int command);
BOOL Comm_HasLinkError();
int Comm_GetMaxPlayers(u32 value);
int Comm_GetMinPlayers(u32 value);
int Comm_GetBattleSlot(int slot);
void CommState_SetUnionAppLinkMode(void);
void CommState_SetUnionRoomLinkMode(void);
int Comm_ParentQueueHasCommand(int value);
BOOL Comm_IsWifiConnected(void);
int Comm_GetBattleSlot(int);
BOOL Comm_IsSendingMoveKeys(void);
void CommState_SetUnionAppLinkMode(void);
void CommState_SetUnionRoomLinkMode(void);

#endif // POKEHEARTGOLD_COMMUNICATION_CORE_H

#ifndef POKEHEARTGOLD_COMMUNICATION_CORE_H
#define POKEHEARTGOLD_COMMUNICATION_CORE_H

#include "global.h"

#include "safari_zone.h"

BOOL Comm_Update(void);
BOOL Comm_IsNetIdConnected(u16 arg0);
BOOL Comm_SendCommand(int arg0, void *arg1, int arg2);
int Comm_CountConnected(void);
BOOL Comm_IsActive(void);
void Comm_EnableKeySync(void);
void Comm_DisableKeySync(void);
BOOL Comm_GetLocalNetId(void);
void Comm_SendFixedCommandNoCopy(s32 arg0, SafariZoneAreaSet *areaSet);
void Comm_SendEmptyCommand(int arg0);
BOOL Comm_HasLinkError();
int Comm_GetMaxPlayers(u32 arg0);
int Comm_GetMinPlayers(u32 arg0);
int Comm_GetBattleSlot(int arg0);
void sub_020380F4(void);
void CommState_SetUnionRoomLinkMode(void);
int Comm_ParentQueueHasCommand(int arg0);
BOOL Comm_IsWifiConnected(void);
int Comm_GetBattleSlot(int);
BOOL Comm_IsSendingMoveKeys(void);
void sub_020380F4(void);
void CommState_SetUnionRoomLinkMode(void);

#endif // POKEHEARTGOLD_COMMUNICATION_CORE_H

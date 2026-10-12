#ifndef POKEHEARTGOLD_COMM_WIRELESS_H
#define POKEHEARTGOLD_COMM_WIRELESS_H

#include "player_data.h"

void CommWireless_StartUp(void);
BOOL CommWireless_IsStartedUp(void);
int CommWireless_CountParents();
u32 CommWireless_GetParentSlot(u32 ordinal);
u8 CommWireless_IsParentListChanged();
void CommWireless_ClearParentListChanged();
void CommWireless_CopyParentProfile(u16 ordinal, PlayerProfile *profile);
BOOL CommWireless_Exists(void);
void CommWireless_SetLostWhenAlone(u32 lostWhenAlone);

#endif // POKEHEARTGOLD_COMM_WIRELESS_H

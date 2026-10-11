#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_RANGER_LINK_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_RANGER_LINK_H

#include "global.h"

// Wireless link to Pokemon Ranger used by Connect to Pokemon Ranger (src/application/main_menu/ranger_link.c).

void RangerLink_Init(const void *config, void *buffer);
void RangerLink_Update(void);
void RangerLink_StartChild(void);
BOOL RangerLink_CanSelectParent(int index);
void RangerLink_SelectParent(int index);
int RangerLink_GetLinkLevel(void);
int RangerLink_CountPeers(void);
void RangerLink_StartSendList(void);
void RangerLink_StartSendNames(void);
void RangerLink_StartCommand(const void *data, int size, int command);
BOOL RangerLink_IsSendDone(void);
BOOL RangerLink_IsCommandDone(void);
BOOL RangerLink_IsCommandAcknowledged(void);
BOOL RangerLink_IsConnected(void);
void RangerLink_RequestEnd(void);
BOOL RangerLink_IsFinished(void);
void RangerLink_SetChannel(u16 channel);
u32 RangerLink_ReadMacAddressTail(u8 *macAddress);
void RangerLink_SplitDecimal(u32 value, u32 *low, u32 *high);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_RANGER_LINK_H

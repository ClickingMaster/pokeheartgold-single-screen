#ifndef POKEHEARTGOLD_COMM_WIRELESS_H
#define POKEHEARTGOLD_COMM_WIRELESS_H

#include "player_data.h"

void CommWireless_StartUp(void);
void CommWireless_Terminate(void);
BOOL CommWireless_IsStartedUp(void);
BOOL CommWireless_IsOn(void);
void CommWireless_ClearParents(void);
BOOL CommWireless_StartParent(int, BOOL, BOOL);
BOOL CommWireless_StartScan(int, BOOL);
BOOL CommWireless_Stop(void);
BOOL CommWireless_RequestStopAndFree(void);
void CommWireless_SetManagerEnded(BOOL);
int CommWireless_CountParents(void);
int CommWireless_GetParentSlot(int ordinal);
u8 CommWireless_IsParentListChanged(void);
void CommWireless_ClearParentListChanged(void);
BOOL CommWireless_GetParentMemberCount(int);
void CommWireless_CopyParentProfile(int ordinal, PlayerProfile *profile);
BOOL CommWireless_ConnectToParent(int);
void CommWireless_UpdateParents(void);
BOOL CommWireless_Main(int);
int CommWireless_IsManagerEnded(void);
BOOL CommWireless_Exists(void);
int CommWireless_IsIdle(void);
int CommWireless_GetConnectedChildren(void);
BOOL CommWireless_IsLinkLost(void);
void CommWireless_SetLostWhenAlone(BOOL value);
void CommWireless_SetDetectDrops(BOOL);
void *CommWireless_GetProfile(void);
void CommWireless_SetMacAddress(void *, int);
BOOL CommWireless_IsShutdownRequested(void);
void CommWireless_RequestShutdown(void);
void CommWireless_SetMail(void *);
void CommWireless_SetRuleset(void *);
void CommWireless_RefreshBeacon(void);
void CommWireless_SetCustomBeacon(const void *);
void *CommWireless_GetParentMail(int);
void CommWireless_Init(PlayerProfile *, int);
BOOL CommWireless_IsInitialized(void);

#endif // POKEHEARTGOLD_COMM_WIRELESS_H

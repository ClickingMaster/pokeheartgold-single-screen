#ifndef POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H
#define POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H

#include "field_system.h"
#include "script.h"

u8 CommPlayerManager_GetMoveTimer(BOOL netId);
void CommClubLink_StartParent(FieldSystem *fieldSystem, s32 linkMode, s32 beaconId);
void CommClubLink_StartChild(FieldSystem *fieldSystem, u8, u32);
void CommClubLink_ConnectToParent(u16 mode);
void CommClubLink_RestartScan();
void CommClubLink_StartCardExchange(FieldSystem *fieldSystem);
void CommClubLink_Exit(void);
void CommClubLink_ExitSynced(void);
void CommClubLink_RecvOnSpot(void);
BOOL CommClubLink_IsWalkingInRoom(void);
void CommClubLink_SetCardReceived(void);
void CommClubLink_GetCard(void);
int HandleNumberSelectInput(s16 *quantity, u16 max);

#endif // POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H

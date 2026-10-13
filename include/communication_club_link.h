#ifndef POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H
#define POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H

#include "field_system.h"
#include "script.h"

u8 CommPlayerManager_GetMoveTimer(BOOL arg0);
void CommClubLink_StartParent(FieldSystem *fieldSystem, s32 arg1, s32 arg2);
void CommClubLink_StartChild(FieldSystem *fieldSystem, u8, u32);
void CommClubLink_ConnectToParent(u16 arg0);
void CommClubLink_RestartScan();
void CommClubLink_StartCardExchange(FieldSystem *fieldSystem);
void CommClubLink_Exit(void);
void CommClubLink_ExitSynced(void);
void CommClubLink_RecvOnSpot(void);
BOOL CommClubLink_IsWalkingInRoom(void);
void CommClubLink_SetCardReceived(void);
void CommClubLink_GetCard(void);
int HandleNumberSelectInput(s16 *quantity, u16 a1);

#endif // POKEHEARTGOLD_COMMUNICATION_CLUB_LINK_H

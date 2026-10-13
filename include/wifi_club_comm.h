#ifndef POKEHEARTGOLD_WIFI_CLUB_COMM_H
#define POKEHEARTGOLD_WIFI_CLUB_COMM_H

#include "global.h"

#include "overlay_44.h"

void WifiClubComm_RegisterCommands(void *);
void WifiClubComm_RegisterCommandsNoContext(void);
s32 WifiClubComm_GetStartCommandSize(void);
s32 WifiClubComm_GetJoinAnswerSize(void);
s32 WifiClubComm_GetVoiceChatFlagsSize(void);
void WifiClubComm_RecvJoinAnswer(void *arg0, void *arg1, String *string, WifiClubGameEntry *entry);
void WifiClubComm_RecvStartCommand(void *arg0, void *arg1, String *arg2, WifiClubGameEntry *entry);
void WifiClubComm_RecvVoiceChatFlags(void *arg0, void *arg1, String *string, WifiClubGameEntry *entry);

#endif // POKEHEARTGOLD_WIFI_CLUB_COMM_H

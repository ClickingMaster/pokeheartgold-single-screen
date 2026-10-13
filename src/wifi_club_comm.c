#include "wifi_club_comm.h"

#include "communication_command.h"
#include "unk_0203AC88.h"

void WifiClubComm_RegisterCommands(void *arg0) {
    Comm_RegisterCommandTable(WifiClubComm_GetCommandTable(), WifiClubComm_GetCommandCount(), arg0);
}

void WifiClubComm_RegisterCommandsNoContext(void) {
    Comm_RegisterCommandTable(WifiClubComm_GetCommandTable(), WifiClubComm_GetCommandCount(), NULL);
}

s32 WifiClubComm_GetStartCommandSize(void) {
    return 0;
}

s32 WifiClubComm_GetJoinAnswerSize(void) {
    return 8;
}

s32 WifiClubComm_GetVoiceChatFlagsSize(void) {
    return 4;
}

void WifiClubComm_RecvJoinAnswer(void *arg0, void *arg1, String *string, WifiClubGameEntry *entry) {
    if (entry != NULL) {
        WifiClubGameEntry_OnJoinAnswer(entry, string);
    }
}

void WifiClubComm_RecvStartCommand(void *arg0, void *arg1, String *arg2, WifiClubGameEntry *entry) {
    if (entry != NULL) {
        WifiClubGameEntry_OnStartCommand(entry);
    }
}

void WifiClubComm_RecvVoiceChatFlags(void *arg0, void *arg1, String *string, WifiClubGameEntry *entry) {
    if (entry != NULL) {
        WifiClubGameEntry_OnVoiceChatFlags(entry, string);
    }
}

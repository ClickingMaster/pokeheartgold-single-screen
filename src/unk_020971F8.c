#include "unk_020971F8.h"

#include "communication_command.h"
#include "unk_0203AC88.h"

void WifiClubComm_RegisterCommands(void *arg0) {
    Comm_RegisterCommandTable(WifiClubComm_GetCommandTable(), WifiClubComm_GetCommandCount(), arg0);
}

void WifiClubComm_RegisterCommandsNoContext(void) {
    Comm_RegisterCommandTable(WifiClubComm_GetCommandTable(), WifiClubComm_GetCommandCount(), NULL);
}

s32 sub_0209722C(void) {
    return 0;
}

s32 sub_02097230(void) {
    return 8;
}

s32 sub_02097234(void) {
    return 4;
}

void sub_02097238(void *arg0, void *arg1, String *arg2, WifiClubGameEntry *arg3) {
    if (arg3 != NULL) {
        WifiClubGameEntry_OnJoinAnswer(arg3, arg2);
    }
}

void sub_02097248(void *arg0, void *arg1, String *arg2, WifiClubGameEntry *arg3) {
    if (arg3 != NULL) {
        WifiClubGameEntry_OnStartCommand(arg3);
    }
}

void sub_02097258(void *arg0, void *arg1, String *arg2, WifiClubGameEntry *arg3) {
    if (arg3 != NULL) {
        WifiClubGameEntry_OnVoiceChatFlags(arg3, arg2);
    }
}

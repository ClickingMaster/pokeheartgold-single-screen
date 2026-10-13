#include "unk_0203AC88.h"

#include "global.h"

#include "wifi_club_comm.h"

static const WifiClubCommCommand sWifiClubCommCommands[] = {
    { WifiClubComm_RecvJoinAnswer, WifiClubComm_GetJoinAnswerSize, NULL },
    { WifiClubComm_RecvStartCommand, WifiClubComm_GetStartCommandSize, NULL },
    { WifiClubComm_RecvVoiceChatFlags, WifiClubComm_GetVoiceChatFlagsSize, NULL },
};

const WifiClubCommCommand *WifiClubComm_GetCommandTable(void) {
    return sWifiClubCommCommands;
}

int WifiClubComm_GetCommandCount(void) {
    return NELEMS(sWifiClubCommCommands);
}

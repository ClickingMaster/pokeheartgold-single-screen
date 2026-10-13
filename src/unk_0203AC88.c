#include "unk_0203AC88.h"

#include "global.h"

#include "unk_020971F8.h"

static const UnkStruct_0203AC88 sWifiClubCommCommands[] = {
    { sub_02097238, sub_02097230, NULL },
    { sub_02097248, sub_0209722C, NULL },
    { sub_02097258, sub_02097234, NULL },
};

const UnkStruct_0203AC88 *WifiClubComm_GetCommandTable(void) {
    return sWifiClubCommCommands;
}

int WifiClubComm_GetCommandCount(void) {
    return NELEMS(sWifiClubCommCommands);
}

#include "unk_02069528.h"

#include "global.h"

#include "frontier_data.h"
#include "player_data.h"
#include "save_wifi_history.h"
#include "frontier_data_access.h"
#include "union_room_avatar.h"

void sub_02069528(SaveData *saveData, s32 a1, UnkStruct_02069528 *a2) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(saveData);
    SaveWiFiHistory *saveWiFiHistory = Save_WiFiHistory_Get(saveData);
    FrontierData *frontierData = Save_FrontierData_Get(saveData);
    MI_CpuFill8(a2, 0, sizeof(UnkStruct_02069528));
    MI_CpuCopy8(PlayerProfile_GetNamePtr(playerProfile), &a2->playerName, (PLAYER_NAME_LENGTH + 1) * sizeof(u16));
    a2->trainerId = PlayerProfile_GetTrainerID(playerProfile);
    a2->gameVersion = gGameVersion;
    a2->gameLanguage = gGameLanguage;
    a2->playerCountry = WifiHistory_GetPlayerCountry(saveWiFiHistory);
    a2->playerRegion = WiFiHistory_GetPlayerRegion(saveWiFiHistory);
    a2->playerGender = PlayerProfile_GetTrainerGender(playerProfile);
    a2->trainerClass = GetUnionRoomAvatarAttrBySprite(a2->playerGender, PlayerProfile_GetAvatar(playerProfile), 1);

    for (s32 index = 0, position = 0; index < 3; index++, position += 8) {
        MI_CpuCopy8(Save_Frontier_GetTowerMessage(saveData, index), &a2->towerMessages[position], 8);
    }

    MI_CpuCopy8(Save_Frontier_GetTowerMessage(saveData, 3), &a2->unkC0, sizeof(a2->unkC0));

    if (a1 == 0) {
        a2->unkE2 = FrontierData_FlagAction(frontierData, 7, 0);
        if (a2->unkE2 != 0) {
            FrontierData_GetWifiTeam(frontierData, a1, a2);
        }
    } else {
        a2->unkE2 = FrontierData_GetWifiScore(frontierData);
        FrontierData_GetWifiTeam(frontierData, a1, a2);
    }
}

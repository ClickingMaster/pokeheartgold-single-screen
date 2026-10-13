#include "unk_02069528.h"

#include "global.h"

#include "frontier_data.h"
#include "player_data.h"
#include "save_wifi_history.h"
#include "frontier_data_access.h"
#include "union_room_avatar.h"

void sub_02069528(SaveData *saveData, s32 a1, FrontierWifiUploadRecord *record) {
    PlayerProfile *playerProfile = Save_PlayerData_GetProfile(saveData);
    SaveWiFiHistory *saveWiFiHistory = Save_WiFiHistory_Get(saveData);
    FrontierData *frontierData = Save_FrontierData_Get(saveData);
    MI_CpuFill8(record, 0, sizeof(FrontierWifiUploadRecord));
    MI_CpuCopy8(PlayerProfile_GetNamePtr(playerProfile), &record->playerName, (PLAYER_NAME_LENGTH + 1) * sizeof(u16));
    record->trainerId = PlayerProfile_GetTrainerID(playerProfile);
    record->gameVersion = gGameVersion;
    record->gameLanguage = gGameLanguage;
    record->playerCountry = WifiHistory_GetPlayerCountry(saveWiFiHistory);
    record->playerRegion = WiFiHistory_GetPlayerRegion(saveWiFiHistory);
    record->playerGender = PlayerProfile_GetTrainerGender(playerProfile);
    record->trainerClass = GetUnionRoomAvatarAttrBySprite(record->playerGender, PlayerProfile_GetAvatar(playerProfile), 1);

    for (s32 index = 0, position = 0; index < 3; index++, position += 8) {
        MI_CpuCopy8(Save_Frontier_GetTowerMessage(saveData, index), &record->towerMessages[position], 8);
    }

    MI_CpuCopy8(Save_Frontier_GetTowerMessage(saveData, 3), &record->unkC0, sizeof(record->unkC0));

    if (a1 == 0) {
        record->unkE2 = FrontierData_FlagAction(frontierData, 7, 0);
        if (record->unkE2 != 0) {
            FrontierData_GetWifiTeam(frontierData, a1, record);
        }
    } else {
        record->unkE2 = FrontierData_GetWifiScore(frontierData);
        FrontierData_GetWifiTeam(frontierData, a1, record);
    }
}

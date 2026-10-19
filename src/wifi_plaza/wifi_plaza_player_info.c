#include "global.h"

#include "wifi_plaza/wifi_plaza_internal.h"

#include "heap.h"
#include "player_data.h"

BOOL WifiConnection_SetOwnStatusData(void *data, s32 size);

WiFiPlazaPlayerInfo *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapId) {
    WiFiPlazaPlayerInfo *info = Heap_Alloc(heapId, sizeof(WiFiPlazaPlayerInfo));
    memset(info, 0, sizeof(WiFiPlazaPlayerInfo));
    info->status = 28;
    info->gender = PlayerProfile_GetTrainerGender(profile);
    info->avatar = PlayerProfile_GetAvatar(profile);
    info->version = PlayerProfile_GetVersion(profile);
    info->language = PlayerProfile_GetLanguage(profile);
    GF_ASSERT(WifiConnection_SetOwnStatusData(info, sizeof(WiFiPlazaPlayerInfo)) == 1);
    return info;
}

void WiFiPlazaPlayerInfo_Free(WiFiPlazaPlayerInfo *info) {
    Heap_Free(info);
}

#include "global.h"

#include "heap.h"
#include "player_data.h"

typedef struct WiFiPlazaPlayerInfo {
    u8 filler_00[0x18];
    u8 version;
    u8 language;
    u8 filler_1A;
    u8 unk1B;
    u8 filler_1C;
    u8 avatar;
    u8 gender;
    u8 filler_1F[5];
} WiFiPlazaPlayerInfo;

BOOL WifiConnection_SetOwnStatusData(void *data, s32 size);
WiFiPlazaPlayerInfo *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapId);
void WiFiPlazaPlayerInfo_Free(void *info);

WiFiPlazaPlayerInfo *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapId) {
    WiFiPlazaPlayerInfo *info = Heap_Alloc(heapId, sizeof(WiFiPlazaPlayerInfo));
    memset(info, 0, sizeof(WiFiPlazaPlayerInfo));
    info->unk1B = 0x1C;
    info->gender = PlayerProfile_GetTrainerGender(profile);
    info->avatar = PlayerProfile_GetAvatar(profile);
    info->version = PlayerProfile_GetVersion(profile);
    info->language = PlayerProfile_GetLanguage(profile);
    GF_ASSERT(WifiConnection_SetOwnStatusData(info, sizeof(WiFiPlazaPlayerInfo)) == 1);
    return info;
}

void WiFiPlazaPlayerInfo_Free(void *info) {
    Heap_Free(info);
}

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

BOOL ov00_021E6D60(void *arg0, s32 arg1);
WiFiPlazaPlayerInfo *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapId);
void WiFiPlazaPlayerInfo_Free(void *ptr);

WiFiPlazaPlayerInfo *WiFiPlazaPlayerInfo_New(PlayerProfile *profile, enum HeapID heapId) {
    WiFiPlazaPlayerInfo *ptr = Heap_Alloc(heapId, 0x24);
    memset(ptr, 0, 0x24);
    ptr->unk1B = 0x1C;
    ptr->gender = PlayerProfile_GetTrainerGender(profile);
    ptr->avatar = PlayerProfile_GetAvatar(profile);
    ptr->version = PlayerProfile_GetVersion(profile);
    ptr->language = PlayerProfile_GetLanguage(profile);
    GF_ASSERT(ov00_021E6D60(ptr, 0x24) == 1);
    return ptr;
}

void WiFiPlazaPlayerInfo_Free(void *ptr) {
    Heap_Free(ptr);
}

#include "global.h"

#include "heap.h"
#include "player_data.h"

typedef struct UnkStruct_02078DD8 {
    u8 filler_00[0x18];
    u8 unk18;
    u8 unk19;
    u8 filler_1A;
    u8 unk1B;
    u8 filler_1C;
    u8 unk1D;
    u8 unk1E;
    u8 filler_1F[5];
} UnkStruct_02078DD8;

BOOL ov00_021E6D60(void *arg0, s32 arg1);
UnkStruct_02078DD8 *sub_02078DD8(PlayerProfile *profile, enum HeapID heapId);
void sub_02078E28(void *ptr);

UnkStruct_02078DD8 *sub_02078DD8(PlayerProfile *profile, enum HeapID heapId) {
    UnkStruct_02078DD8 *ptr = Heap_Alloc(heapId, 0x24);
    memset(ptr, 0, 0x24);
    ptr->unk1B = 0x1C;
    ptr->unk1E = PlayerProfile_GetTrainerGender(profile);
    ptr->unk1D = PlayerProfile_GetAvatar(profile);
    ptr->unk18 = PlayerProfile_GetVersion(profile);
    ptr->unk19 = PlayerProfile_GetLanguage(profile);
    GF_ASSERT(ov00_021E6D60(ptr, 0x24) == 1);
    return ptr;
}

void sub_02078E28(void *ptr) {
    Heap_Free(ptr);
}

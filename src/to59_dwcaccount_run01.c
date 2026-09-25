#include "global.h"
#pragma thumb off
#pragma require_prototypes off

typedef struct {
    u32 word0;
    u32 word1;
    u32 word2;
} DWCAcc;

extern const char _version_NINTENDO_DWC[];
void OSi_ReferSymbol(const void *);
int DWC_BM_Init(void *);
int DWC_Auth_CheckPseudoWiFiID(void);
void DWCi_AUTH_MakeWiFiID(void *);

int DWC_Init(void *data) {
    int result;
    int made = 0;
    OSi_ReferSymbol(_version_NINTENDO_DWC);
    result = DWC_BM_Init(data);
    if (DWC_Auth_CheckPseudoWiFiID()) {
        DWCi_AUTH_MakeWiFiID(data);
        made = 1;
    }
    if (result < 0) {
        return made ? 2 : 3;
    }
    return made ? 1 : 0;
}

int DWCi_Acc_SetMaskBits(u32 *word, u32 bits, int shift, u32 mask) {
    if (bits & ~mask) {
        return 0;
    }
    *word = (*word & ~(mask << shift)) | (bits << shift);
    return 1;
}

u64 DWCi_Acc_GetUserId(DWCAcc *data) {
    u32 low = data->word0;
    u32 high = data->word1;
    u32 mask = low & 0x7ff;
    return ((u64)mask << 32) | high;
}
u32 DWCi_Acc_GetPlayerId(DWCAcc *data) {
    return data->word2;
}
u64 DWCi_Acc_GetFriendKey(DWCAcc *data) {
    return ((u64)data->word2 << 32) | data->word1;
}
u32 DWCi_Acc_GetGsProfileId(DWCAcc *data) {
    return data->word1;
}
void DWCi_Acc_SetUserId(DWCAcc *data, u32 high, u32 low) {
    DWCi_Acc_SetMaskBits(&data->word0, low, 0, 0x7ff);
    data->word1 = high;
}
void DWCi_Acc_SetPlayerId(DWCAcc *data, u32 id) {
    data->word2 = id;
}
void DWCi_Acc_SetFriendKey(DWCAcc *data, u64 key) {
    data->word1 = (u32)key;
    data->word2 = (u32)(key >> 32);
}
void DWCi_Acc_SetGsProfileId(DWCAcc *data, u32 id) {
    data->word1 = id;
}
u32 DWCi_Acc_GetFlags(DWCAcc *data) {
    return (data->word0 >> 11) & 0x1fffff;
}
u32 DWCi_Acc_GetFlag_DataType(DWCAcc *data) {
    return DWCi_Acc_GetFlags(data) & 3;
}
int DWC_IsBuddyFriendData(DWCAcc *data) {
    if (DWCi_Acc_GetFlag_DataType(data) == 3) {
        return (DWCi_Acc_GetFlags(data) & 4) == 4;
    }
    return 0;
}
u32 DWC_GetFriendDataType(DWCAcc *data) {
    return DWCi_Acc_GetFlag_DataType(data);
}
int DWCi_Acc_SetFlags(DWCAcc *data, u32 bits) {
    return DWCi_Acc_SetMaskBits(&data->word0, bits, 11, 0x1fffff);
}
void DWCi_Acc_SetFlag_DataType(DWCAcc *data, u32 type) {
    u32 flags = DWCi_Acc_GetFlags(data);
    DWCi_Acc_SetFlags(data, (flags & ~3) | type);
}

int DWC_Acc_CheckFriendKey(u32 id, u32 crc, u32 profile);

extern const char _021105F4[];

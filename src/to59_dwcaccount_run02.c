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

u64 DWC_Acc_CreateFriendKey(u32 id, u32 profile) {
    struct MATHCRC8Table table;
    u32 pair[2];
    pair[0] = id;
    pair[1] = profile;
    MATHi_CRC8InitTable(&table, 7);
    return ((u64)(MATH_CalcCRC8(&table, pair, 8) & 0x7f) << 32) | id;
}

int DWC_Acc_CheckFriendKey(u32 id, u32 crc, u32 profile);
int DWC_CheckFriendKey(u8 *data, u32 id, u32 crc) {
    return DWC_Acc_CheckFriendKey(id, crc, *(u32 *)(data + 0x24));
}

extern const char _021105F4[];

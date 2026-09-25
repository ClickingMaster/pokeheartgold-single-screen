typedef unsigned long u32;
typedef unsigned char u8;
typedef unsigned long long u64;

int DWCi_Acc_GetFlag_DataType(const void *);
void DWCi_Acc_CreateUserData(void *, u32);
int DWCi_Acc_IsValidLoginId(const void *);
int DWC_IsValidFriendData(const void *);
int DWCi_Acc_IsValidFriendData(const void *);
void DWC_CreateUserData(void *, u32);
int DWC_CheckDirtyFlag(const void *);
int DWCi_Acc_IsDirty(const void *);
void DWC_ClearDirtyFlag(void *);
void DWCi_Acc_ClearDirty(void *);
int DWC_CheckHasProfile(const void *);
u64 DWC_CreateFriendKey(const void *);
u64 DWC_Acc_CreateFriendKey(u32, u32);

int DWCi_Acc_IsAuthentic(const void *data);

void MATHi_CRC32InitTableRev(void *, u32);
u32 MATH_CalcCRC32(void *, const void *, u32);
int DWC_CheckUserData(const void *);
void DWCi_Acc_ClearDirty(void *);
void DWCi_Acc_LoginIdToUserName(const void *, u32, char *);
void DWC_LoginIdToUserName(const void *, const void *, char *);

void MI_CpuFill8(void *, u8, u32);
void DWCi_Acc_SetFriendKey(void *, u32, u32);
void DWCi_Acc_SetFlag_DataType(void *, int);
void DWCi_Acc_SetGsProfileId(void *, u32);
void DWC_CreateFriendKeyToken(void *, u32, u32);
void DWC_CreateExchangeToken(const void *, void *);
void DWC_SetGsProfileId(void *, u32);

typedef struct {
    u32 a, b, c;
} ThreeWords;

void DWCi_Acc_CreateTempLoginId(void *);
void DWCi_Acc_SetLoginIdToUserData(void *, const ThreeWords *, u32);

u64 DWCi_Acc_GetUserId(const void *);
u64 DWCi_Acc_GetFriendKey(const void *);
void DWC_Auth_GetId(u32 *);
u64 DWC_GetFriendKey(const void *);
int DWCi_Acc_CheckConsoleUserId(const void *);
int DWC_CheckValidConsole(const void *);

int DWCi_Acc_GetGsProfileId(const void *);
int DWC_Acc_CheckFriendKey(u64, u32);
int DWC_Acc_FriendKeyToGsProfileId(u64, u32);
int DWC_GetGsProfileId(const void *, const void *);

u32 DWCi_Acc_GetPlayerId(const void *);
int DWC_IsEqualFriendData(const void *, const void *);

extern const char _02110618[];
void DWCi_Acc_U64ToString32(u64, int, char *);
int OS_SNPrintf(char *, u32, const char *, ...);
void DWCi_Acc_LoginIdToUserName(const void *, u32, char *);

void OS_GetLowEntropyData(u32 *);
void DWCi_Acc_SetUserId(void *, u32, u32);
void DWCi_Acc_SetPlayerId(void *, u32);
void DWCi_Acc_CreateTempLoginId(void *);

void DWCi_Acc_LoginIdToUserName(const void *loginId, u32 gameCode, char *out) {
    char temp[0x2c];
    DWCi_Acc_U64ToString32(DWCi_Acc_GetUserId(loginId), 0x2b, temp + 0x15);
    DWCi_Acc_U64ToString32(DWCi_Acc_GetPlayerId(loginId), 0x20, temp);
    OS_SNPrintf(out, 0x15, _02110618, temp + 0x15, (gameCode >> 24) & 0xff, (gameCode >> 16) & 0xff, (gameCode >> 8) & 0xff, gameCode & 0xff, temp);
}

void DWCi_Acc_CreateUserData(void *data, u32 gameCode) {
    u32 table[256];
    MI_CpuFill8(data, 0, 0x40);
    *(u32 *)data = 0x40;
    *(u32 *)((u8 *)data + 0x1c) = 0;
    *(u32 *)((u8 *)data + 0x24) = gameCode;
    DWCi_Acc_CreateTempLoginId((u8 *)data + 4);
    DWCi_Acc_SetFlag_DataType((u8 *)data + 0x10, 0);
    MATHi_CRC32InitTableRev(table, 0xEDB88320);
    *(u32 *)((u8 *)data + 0x3c) = MATH_CalcCRC32(table, data, 0x3c);
    *(u32 *)((u8 *)data + 0x20) |= 1;
}

void DWCi_Acc_CreateTempLoginId(void *data) {
    u32 auth[5];
    u32 entropy[8];
    u32 i;
    u64 state;
    OS_GetLowEntropyData(entropy);
    for (i = 1; i < 8; i++) {
        entropy[i] ^= entropy[i - 1];
    }
    state = entropy[7];
    DWC_Auth_GetId(auth);
    if (auth[4]) {
        DWCi_Acc_SetUserId(data, auth[0], auth[1]);
    } else {
        DWCi_Acc_SetUserId(data, auth[2], auth[3]);
    }
    state = state * 0x5D588B656C078965ULL + 0x269EC3ULL;
    DWCi_Acc_SetPlayerId(data, state >> 32);
    DWCi_Acc_SetFlag_DataType(data, 1);
}

int DWCi_Acc_CheckConsoleUserId(const void *loginId) {
    u32 auth[5];
    DWC_Auth_GetId(auth);
    if (auth[4]) {
        return DWCi_Acc_GetUserId(loginId) == *(u64 *)&auth[0];
    }
    return DWCi_Acc_GetUserId(loginId) == *(u64 *)&auth[2];
}

int DWCi_Acc_IsValidLoginId(const void *id) {
    return DWCi_Acc_GetFlag_DataType(id) == 1;
}

int DWCi_Acc_IsAuthentic(const void *data) {
    return DWCi_Acc_IsValidLoginId((const u8 *)data + 0x10);
}

int DWC_IsValidFriendData(const void *data) {
    return DWCi_Acc_GetFlag_DataType(data) != 0;
}

int DWCi_Acc_IsValidFriendData(const void *data) {
    return DWC_IsValidFriendData(data);
}

void DWC_CreateUserData(void *data, u32 id) {
    DWCi_Acc_CreateUserData(data, id);
}

int DWC_CheckUserData(const void *data) {
    u32 table[256];
    MATHi_CRC32InitTableRev(table, 0xEDB88320);
    u32 crc = MATH_CalcCRC32(table, data, 0x3c);
    u32 stored = *(const u32 *)((const u8 *)data + 0x3c);
    if (crc == stored) {
        return 1;
    }
    return 0;
}

int DWC_CheckHasProfile(const void *data) {
    if (DWCi_Acc_IsValidLoginId((const u8 *)data + 0x10)) {
        if (*(const int *)((const u8 *)data + 0x1c) > 0) {
            return 1;
        }
    }
    return 0;
}

int DWC_CheckValidConsole(const void *data) {
    u32 auth[5];
    if (DWCi_Acc_GetFlag_DataType((const u8 *)data + 0x10) == 0) {
        return 1;
    }
    DWC_Auth_GetId(auth);
    if (auth[4] == 0) {
        return 0;
    }
    if (DWCi_Acc_GetUserId((const u8 *)data + 0x10) == *(u64 *)&auth[0]) {
        return 1;
    }
    return 0;
}

void DWCi_Acc_SetLoginIdToUserData(void *data, const ThreeWords *id, u32 profile) {
    u32 table[256];
    *(ThreeWords *)((u8 *)data + 0x10) = *id;
    *(u32 *)((u8 *)data + 0x1c) = profile;
    MATHi_CRC32InitTableRev(table, 0xEDB88320);
    *(u32 *)((u8 *)data + 0x3c) = MATH_CalcCRC32(table, data, 0x3c);
    *(u32 *)((u8 *)data + 0x20) |= 1;
}

int DWC_CheckDirtyFlag(const void *data) {
    return DWCi_Acc_IsDirty(data);
}

int DWCi_Acc_IsDirty(const void *data) {
    return (*(const u32 *)((const u8 *)data + 0x20) & 1) == 1;
}

void DWC_ClearDirtyFlag(void *data) {
    DWCi_Acc_ClearDirty(data);
}

void DWCi_Acc_ClearDirty(void *data) {
    u32 table[256];
    *(u32 *)((u8 *)data + 0x20) &= ~1u;
    MATHi_CRC32InitTableRev(table, 0xEDB88320);
    *(u32 *)((u8 *)data + 0x3c) = MATH_CalcCRC32(table, data, 0x3c);
}

u64 DWC_GetFriendKey(const void *data) {
    if (DWCi_Acc_GetFlag_DataType(data) != 2) {
        return 0;
    }
    return DWCi_Acc_GetFriendKey(data);
}

int DWC_GetGsProfileId(const void *user, const void *friendData) {
    switch (DWCi_Acc_GetFlag_DataType(friendData)) {
    case 2: {
        u64 key = DWCi_Acc_GetFriendKey(friendData);
        if (!DWC_Acc_CheckFriendKey(key, *(const u32 *)((const u8 *)user + 0x24))) {
            return 0;
        }
        return DWC_Acc_FriendKeyToGsProfileId(key, *(const u32 *)((const u8 *)user + 0x24));
    }
    case 3:
        return DWCi_Acc_GetGsProfileId(friendData);
    case 1:
        return -1;
    default:
        return 0;
    }
}

u64 DWC_CreateFriendKey(const void *data) {
    u32 profile = *(const u32 *)((const u8 *)data + 0x1c);
    u64 key = 0;
    if (profile != 0) {
        key = DWC_Acc_CreateFriendKey(profile, *(const u32 *)((const u8 *)data + 0x24));
    }
    return key;
}

void DWC_CreateFriendKeyToken(void *data, u32 low, u32 high) {
    MI_CpuFill8(data, 0, 12);
    DWCi_Acc_SetFriendKey(data, low, high);
    DWCi_Acc_SetFlag_DataType(data, 2);
}

void DWC_CreateExchangeToken(const void *user, void *token) {
    MI_CpuFill8(token, 0, 12);
    if (DWCi_Acc_IsAuthentic(user)) {
        DWCi_Acc_SetGsProfileId(token, *(const u32 *)((const u8 *)user + 0x1c));
        DWCi_Acc_SetFlag_DataType(token, 3);
    } else {
        *(ThreeWords *)token = *(const ThreeWords *)((const u8 *)user + 4);
    }
}

void DWC_SetGsProfileId(void *data, u32 id) {
    MI_CpuFill8(data, 0, 12);
    DWCi_Acc_SetGsProfileId(data, id);
    DWCi_Acc_SetFlag_DataType(data, 3);
}

void DWC_LoginIdToUserName(const void *data, const void *id, char *name) {
    DWCi_Acc_LoginIdToUserName(id, *(const u32 *)((const u8 *)data + 0x24), name);
}

int DWC_IsEqualFriendData(const void *first, const void *second) {
    int type = DWCi_Acc_GetFlag_DataType(first);
    if (type != DWCi_Acc_GetFlag_DataType(second)) {
        return 0;
    }
    if (type == 3) {
        return DWCi_Acc_GetGsProfileId(first) == DWCi_Acc_GetGsProfileId(second);
    }
    if (type == 1) {
        if (DWCi_Acc_GetUserId(first) == DWCi_Acc_GetUserId(second)) {
            if (DWCi_Acc_GetPlayerId(first) == DWCi_Acc_GetPlayerId(second)) {
                return 1;
            }
        }
        return 0;
    }
    if (type == 2) {
        return DWCi_Acc_GetFriendKey(first) == DWCi_Acc_GetFriendKey(second);
    }
    return 0;
}

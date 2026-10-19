#include "frontier_save_access.h"

enum HallChallengeField {
    HALL_CHALLENGE_TYPE,
    HALL_CHALLENGE_BATTLE_NO,
    HALL_CHALLENGE_TRAINER_IDS,
    HALL_CHALLENGE_PARTY_SLOTS,
    HALL_CHALLENGE_SET_IDS,
    HALL_FLAGS_HAS_STREAK,
};

enum CastleChallengeField {
    CASTLE_CHALLENGE_TYPE,
    CASTLE_CHALLENGE_BATTLE_NO,
    CASTLE_CHALLENGE_HP,
    CASTLE_CHALLENGE_PP,
    CASTLE_CHALLENGE_STATUS,
    CASTLE_CHALLENGE_HELD_ITEM,
    CASTLE_CHALLENGE_TRAINER_IDS,
    CASTLE_CHALLENGE_PARTY_SLOTS,
    CASTLE_CHALLENGE_SET_IDS,
    CASTLE_FLAGS_HAS_STREAK,
};

enum ArcadeChallengeField {
    ARCADE_CHALLENGE_TYPE,
    ARCADE_CHALLENGE_RANDOM_FLAG,
    ARCADE_CHALLENGE_BATTLE_NO,
    ARCADE_CHALLENGE_CURSOR_SPEED,
    ARCADE_CHALLENGE_HELD_ITEMS,
    ARCADE_CHALLENGE_TRAINER_IDS,
    ARCADE_CHALLENGE_PARTY_SLOTS,
    ARCADE_CHALLENGE_SET_IDS,
    ARCADE_FLAGS_HAS_STREAK,
};

enum FactoryFlagsField {
    FACTORY_FLAGS_HAS_STREAK = 10,
};

void *Save_Frontier_GetCastleChallenge(SaveData *save);
void *Save_Frontier_GetHallChallenge(SaveData *save);
void FactoryFlags_Init(void *state);
u32 FactoryFlags_GetField(u8 *state, int field, int index);
void HallChallenge_Clear(void *state);

typedef struct {
    u8 kind : 3, active : 1, rest : 4;
    u8 value;
    u8 bytes[2];
    u16 values[20];
    u16 more[20];
} FrontierState;

u32 HallChallenge_GetSaved(FrontierState *state);
void HallChallenge_SetSaved(FrontierState *state, int value);
void HallChallenge_SetField(FrontierState *state, int field, int index, int unused, void *value);
u32 HallChallenge_GetField(FrontierState *state, int field, int index, int unused, int unused2);
u8 HallTypeRanks_Get(int index, u8 *values);
void HallTypeRanks_Set(int index, u8 *values, int value);
void HallTypeRanks_Clear(u8 *values);
void HallFlags_Init(void *state);

void FactoryFlags_Init(void *state) {
    MI_CpuFill8(state, 0, 4);
}

void FactoryFlags_SetField(u32 state, u32 field, u8 index, u8 *value) {
    if (field == FACTORY_FLAGS_HAS_STREAK) {
        if (*value >= 1) {
            *(u8 *)state |= 1 << index;
        } else {
            *(u8 *)state &= (1 << index) ^ 0xff;
        }
    }
}

u32 FactoryFlags_GetField(u8 *state, int field, int index) {
    if (field == FACTORY_FLAGS_HAS_STREAK) {
        return (*state >> index) & 1;
    }
    return 0;
}

int Save_Frontier_GetFactoryFlags(SaveData *save) {
    return (int)((u8 *)Save_Frontier_GetStatic(save) + 0x1618);
}

void HallChallenge_Clear(void *state) {
    MI_CpuFill8(state, 0, 0x54);
}

void *Save_Frontier_GetHallChallenge(SaveData *save) {
    return (u8 *)Save_Frontier_GetStatic(save) + 0x8e0;
}

u32 HallChallenge_GetSaved(FrontierState *state) {
    return state->active;
}

void HallChallenge_SetSaved(FrontierState *state, int value) {
    state->active = (u8)value;
}

void HallChallenge_SetField(FrontierState *state, int field, int index, int unused, void *value) {
    switch (field) {
    case HALL_CHALLENGE_TYPE:
        state->kind = *(u8 *)value;
        break;
    case HALL_CHALLENGE_BATTLE_NO:
        state->value = *(u8 *)value;
        break;
    case HALL_CHALLENGE_TRAINER_IDS:
        state->values[index] = *(u16 *)value;
        break;
    case HALL_CHALLENGE_PARTY_SLOTS:
        state->bytes[index] = *(u8 *)value;
        break;
    case HALL_CHALLENGE_SET_IDS:
        state->more[index] = *(u16 *)value;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

u32 HallChallenge_GetField(FrontierState *state, int field, int index, int unused, int unused2) {
    switch (field) {
    case HALL_CHALLENGE_TYPE:
        return state->kind;
    case HALL_CHALLENGE_BATTLE_NO:
        return state->value;
    case HALL_CHALLENGE_TRAINER_IDS:
        return state->values[index];
    case HALL_CHALLENGE_PARTY_SLOTS:
        return state->bytes[index];
    case HALL_CHALLENGE_SET_IDS:
        return state->more[index];
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

u8 HallTypeRanks_Get(int index, u8 *values) {
    u8 byte = (index >> 1);
    u8 nibble = index % 2;
    return (values[byte] >> (nibble * 4)) & 15;
}

void HallTypeRanks_Set(int index, u8 *values, int value) {
    u8 byte = (index >> 1);
    u8 nibble = index % 2;
    if (nibble == 0) {
        values[byte] &= 0xf0;
    } else {
        values[byte] &= 0xf;
    }
    values[byte] |= (u8)(value << (nibble * 4));
}

void HallTypeRanks_Clear(u8 *values) {
    for (int i = 0; i < 18; i++) {
        HallTypeRanks_Set((u8)i, values, 0);
    }
}

void HallFlags_Init(void *state) {
    MI_CpuFill8(state, 0, 4);
}

int Save_Frontier_GetHallFlags(SaveData *save) {
    return (int)((u8 *)Save_Frontier_GetStatic(save) + 0x161c);
}

void HallFlags_SetField(u32 address, u32 field, u32 index, u32 unused, void *input) {
    u8 *value = input;
    switch (field) {
    case HALL_FLAGS_HAS_STREAK:
        if (*value >= 1) {
            *(u8 *)address |= 1 << index;
        } else {
            *(u8 *)address &= (1 << index) ^ 0xff;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

int HallFlags_GetField(int address, int field, u8 index, int unused, int unused2) {
    if (field == HALL_FLAGS_HAS_STREAK) {
        return (*(u8 *)address >> index) & 1;
    }
    GF_ASSERT(FALSE);
    return 0;
}

void CastleChallenge_Clear(void *state);
u32 CastleChallenge_GetSaved(FrontierState *state);
void CastleChallenge_SetSaved(FrontierState *state, int value);

void CastleChallenge_Clear(void *state) {
    MI_CpuFill8(state, 0, 0x74);
}

void *Save_Frontier_GetCastleChallenge(SaveData *save) {
    return (u8 *)Save_Frontier_GetStatic(save) + 0x8e0;
}

u32 CastleChallenge_GetSaved(FrontierState *state) {
    return state->active;
}

void CastleChallenge_SetSaved(FrontierState *state, int value) {
    state->active = (u8)value;
}

typedef struct {
    u8 kind : 3, active : 1, rest : 4;
    u8 value, padding;
    u8 bytes[3];
    u16 values6[14];
    u16 values22[4];
    u8 matrix[4][4];
    u16 padding3a;
    u32 values3c[4];
    u16 values4c[4];
    u16 values54[16];
} FrontierState2;

void CastleChallenge_SetField(FrontierState2 *state, int field, int index, int column, void *value);
u32 CastleChallenge_GetField(FrontierState2 *state, int field, int index, int column, int unused);
void CastleFlags_Init(void *state);

void CastleChallenge_SetField(FrontierState2 *state, int field, int index, int column, void *value) {
    switch (field) {
    case CASTLE_CHALLENGE_TYPE:
        state->kind = *(u8 *)value;
        break;
    case CASTLE_CHALLENGE_BATTLE_NO:
        state->value = *(u8 *)value;
        break;
    case CASTLE_CHALLENGE_HP:
        state->values22[index] = *(u16 *)value;
        break;
    case CASTLE_CHALLENGE_PP:
        state->matrix[index][column] = *(u8 *)value;
        break;
    case CASTLE_CHALLENGE_STATUS:
        state->values3c[index] = *(u32 *)value;
        break;
    case CASTLE_CHALLENGE_HELD_ITEM:
        state->values4c[index] = *(u16 *)value;
        break;
    case CASTLE_CHALLENGE_TRAINER_IDS:
        state->values6[index] = *(u16 *)value;
        break;
    case CASTLE_CHALLENGE_PARTY_SLOTS:
        state->bytes[index] = *(u8 *)value;
        break;
    case CASTLE_CHALLENGE_SET_IDS:
        state->values54[index] = *(u16 *)value;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

u32 CastleChallenge_GetField(FrontierState2 *state, int field, int index, int column, int unused) {
    switch (field) {
    case CASTLE_CHALLENGE_TYPE:
        return state->kind;
    case CASTLE_CHALLENGE_BATTLE_NO:
        return state->value;
    case CASTLE_CHALLENGE_HP:
        return state->values22[index];
    case CASTLE_CHALLENGE_PP:
        return state->matrix[index][column];
    case CASTLE_CHALLENGE_STATUS:
        return state->values3c[index];
    case CASTLE_CHALLENGE_HELD_ITEM:
        return state->values4c[index];
    case CASTLE_CHALLENGE_TRAINER_IDS:
        return state->values6[index];
    case CASTLE_CHALLENGE_PARTY_SLOTS:
        return state->bytes[index];
    case CASTLE_CHALLENGE_SET_IDS:
        return state->values54[index];
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

void CastleFlags_Init(void *state) {
    MI_CpuFill8(state, 0, 4);
}

u32 Save_Frontier_GetCastleFlags(SaveData *save) {
    return (u32)((u8 *)Save_Frontier_GetStatic(save) + 0x1620);
}

void CastleFlags_SetField(u32 address, u32 field, u32 index, u32 unused, void *input) {
    u8 *value = input;
    switch (field) {
    case CASTLE_FLAGS_HAS_STREAK:
        if (*value >= 1) {
            *(u8 *)address |= 1 << index;
        } else {
            *(u8 *)address &= (1 << index) ^ 0xff;
        }
        break;
    case 10:
        ((u8 *)address)[1] = 1;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

u32 CastleFlags_GetField(u32 address, u32 field, u8 index, u32 unused, u32 unused2) {
    switch (field) {
    case CASTLE_FLAGS_HAS_STREAK:
        return (*(u8 *)address >> index) & 1;
    case 10:
        return ((u8 *)address)[1];
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

void ArcadeChallenge_Clear(void *state);
void ArcadeChallenge_SetSaved(FrontierState *state, int value);

void ArcadeChallenge_Clear(void *state) {
    MI_CpuFill8(state, 0, 0x48);
}

void *Save_Frontier_GetArcadeChallenge(SaveData *save) {
    return (u8 *)Save_Frontier_GetStatic(save) + 0x8e0;
}

u32 ArcadeChallenge_GetSaved(u32 state) {
    return ((FrontierState *)state)->active;
}

void ArcadeChallenge_SetSaved(FrontierState *state, int value) {
    state->active = (u8)value;
}

typedef struct {
    u8 kind : 3, active : 1, flag4 : 1, rest : 3;
    u8 value1, value2, bytes[3];
    u16 values6[4];
    u16 valuese[14];
    u16 values2a[15];
} FrontierState3;

void ArcadeChallenge_SetField(FrontierState3 *state, int field, int index, int unused, void *value);
u32 ArcadeChallenge_GetField(FrontierState3 *state, int field, int index, int unused, int unused2);
void ArcadeFlags_Init(void *state);

void ArcadeChallenge_SetField(FrontierState3 *state, int field, int index, int unused, void *value) {
    switch (field) {
    case ARCADE_CHALLENGE_TYPE:
        state->kind = *(u8 *)value;
        break;
    case ARCADE_CHALLENGE_RANDOM_FLAG:
        state->flag4 = *(u8 *)value;
        break;
    case ARCADE_CHALLENGE_BATTLE_NO:
        state->value1 = *(u8 *)value;
        break;
    case ARCADE_CHALLENGE_CURSOR_SPEED:
        state->value2 = *(u8 *)value;
        break;
    case ARCADE_CHALLENGE_HELD_ITEMS:
        state->values6[index] = *(u16 *)value;
        break;
    case ARCADE_CHALLENGE_TRAINER_IDS:
        state->valuese[index] = *(u16 *)value;
        break;
    case ARCADE_CHALLENGE_PARTY_SLOTS:
        state->bytes[index] = *(u8 *)value;
        break;
    case ARCADE_CHALLENGE_SET_IDS:
        state->values2a[index] = *(u16 *)value;
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

u32 ArcadeChallenge_GetField(FrontierState3 *state, int field, int index, int unused, int unused2) {
    switch (field) {
    case ARCADE_CHALLENGE_TYPE:
        return state->kind;
    case ARCADE_CHALLENGE_RANDOM_FLAG:
        return state->flag4;
    case ARCADE_CHALLENGE_BATTLE_NO:
        return state->value1;
    case ARCADE_CHALLENGE_CURSOR_SPEED:
        return state->value2;
    case ARCADE_CHALLENGE_HELD_ITEMS:
        return state->values6[index];
    case ARCADE_CHALLENGE_TRAINER_IDS:
        return state->valuese[index];
    case ARCADE_CHALLENGE_PARTY_SLOTS:
        return state->bytes[index];
    case ARCADE_CHALLENGE_SET_IDS:
        return state->values2a[index];
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return 0;
}

void ArcadeFlags_Init(void *state) {
    MI_CpuFill8(state, 0, 4);
}

void *Save_Frontier_GetArcadeFlags(SaveData *save) {
    return (u8 *)Save_Frontier_GetStatic(save) + 0x1624;
}

void ArcadeFlags_SetField(void *address, u32 field, u32 index, u32 unused, void *input) {
    u8 *value = input;
    if (field == ARCADE_FLAGS_HAS_STREAK) {
        if (*value >= 1) {
            *(u8 *)address |= 1 << index;
        } else {
            *(u8 *)address &= (1 << index) ^ 0xff;
        }
    } else {
        GF_ASSERT(FALSE);
    }
}

u32 ArcadeFlags_GetField(void *address, u32 field, u8 index, u32 unused, u32 unused2) {
    if (field == ARCADE_FLAGS_HAS_STREAK) {
        return (*(u8 *)address >> index) & 1;
    }
    GF_ASSERT(FALSE);
    return 0;
}

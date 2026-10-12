#include "frontier_save_access.h"
#include "save_arrays.h"
extern FrontierSave gFrontierSave;
u32 Save_Frontier_sizeof(void);
void Save_Frontier_Init(FrontierSave *frontier);
void FrontierData_Reset(void *data);
void TowerMessages_Init(void *data);
void BattleRoomDownload_Init(void *data);
void FactoryFlags_Init(void *state);
void HallFlags_Init(void *state);
void CastleFlags_Init(void *state);
void ArcadeFlags_Init(void *state);
void FrontierSave_ClearRows(FrontierSave *frontier);
void FrontierSave_DeleteRow(FrontierSave *frontier, int row);
void FrontierSave_ClearRow(FrontierSave *frontier, int row);
u32 FrontierSave_AddStat(FrontierSave *frontier, int stat, int index, u16 amount);
u32 FrontierSave_SubStat(FrontierSave *frontier, int stat, int index, int amount);
u32 FrontierSpeciesRecords_sizeof(void);
void FrontierSpeciesRecords_Init(struct FrontierSpeciesRecords *data);
int FrontierSpeciesRecords_Save(SaveData *save, struct FrontierSpeciesRecords *data);

u32 Save_Frontier_sizeof(void) {
    return sizeof(FrontierSave);
}

void Save_Frontier_Init(FrontierSave *frontier) {
    MI_CpuFill8(frontier, 0, sizeof(*frontier));
    FrontierData_Reset((u8 *)frontier + 0x954);
    TowerMessages_Init((u8 *)frontier + 0xabc);
    BattleRoomDownload_Init((u8 *)frontier + 0xadc);
    FactoryFlags_Init((u8 *)frontier + 0x1618);
    HallFlags_Init((u8 *)frontier + 0x161c);
    CastleFlags_Init((u8 *)frontier + 0x1620);
    ArcadeFlags_Init((u8 *)frontier + 0x1624);
    MI_CpuCopy8(frontier, &gFrontierSave, sizeof(*frontier));
}

FrontierSave *Save_Frontier_GetStatic(SaveData *save) {
    return &gFrontierSave;
}

void Save_Frontier_Commit(SaveData *save) {
    FrontierSave *frontier = SaveArray_Get(save, 19);
    MI_CpuCopy8(&gFrontierSave, frontier, sizeof(*frontier));
}

void Save_Frontier_Load(SaveData *save) {
    FrontierSave *frontier = SaveArray_Get(save, 19);
    MI_CpuCopy8(frontier, &gFrontierSave, sizeof(*frontier));
}

u16 FrontierSave_GetStat(FrontierSave *frontier, int stat, int index) {
    if (stat < 112) {
        if (stat >= 100) {
            if (index >= 16) {
                stat++;
                index -= 16;
            }
            return (frontier->unk_000[stat] >> index) & 1;
        }
        return frontier->unk_000[stat];
    }
    if (index == -1) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return frontier->unk_0E0[index][stat - 112];
}

u32 FrontierSave_SetStat(FrontierSave *frontier, int stat, int index, u16 value) {
    if (value > 9999) {
        value = 9999;
    }
    if (stat < 112) {
        if (stat >= 100) {
            GF_ASSERT(index != 255);
            if (index >= 16) {
                stat++;
                index -= 16;
            }
            if (value == 0) {
                frontier->unk_000[stat] &= (1 << index) ^ 0xffff;
            } else {
                frontier->unk_000[stat] |= 1 << index;
            }
        } else {
            GF_ASSERT(index == 255);
            frontier->unk_000[stat] = value;
        }
    } else {
        GF_ASSERT(index != 255);
        frontier->unk_0E0[index][stat - 112] = value;
    }
    return value;
}

void FrontierSave_ClearRows(FrontierSave *frontier) {
    MI_CpuFill8(frontier->unk_0E0, 0, 2048);
    for (int i = 100; i <= 111; i++) {
        frontier->unk_000[i] = 0;
    }
}

void FrontierSave_DeleteRow(FrontierSave *frontier, int row) {
    GF_ASSERT(row != 255);
    for (; row < 31; row++) {
        MI_CpuCopy8(frontier->unk_0E0[row + 1], frontier->unk_0E0[row], 64);
        for (int i = 100; i < 111; i += 2) {
            FrontierSave_SetStat(frontier, i, row, FrontierSave_GetStat(frontier, i, row + 1));
        }
    }
    frontier = (FrontierSave *)frontier->unk_0E0;
    MI_CpuFill8((u8 *)frontier + (31 << 6), 0, 64);
}

void FrontierSave_ClearRow(FrontierSave *frontier, int row) {
    MI_CpuFill8(frontier->unk_0E0[row], 0, 64);
}

u32 FrontierSave_AddStat(FrontierSave *frontier, int stat, int index, u16 amount) {
    u16 value = FrontierSave_GetStat(frontier, stat, index);
    value += amount;
    return FrontierSave_SetStat(frontier, stat, index, value);
}

u32 FrontierSave_SubStat(FrontierSave *frontier, int stat, int index, int amount) {
    int value = FrontierSave_GetStat(frontier, stat, index) - amount;
    if (value < 0) {
        value = 0;
    }
    return FrontierSave_SetStat(frontier, stat, index, (u16)value);
}

void FrontierSave_RaiseStat(FrontierSave *frontier, u32 stat, u32 index, u16 value) {
    u32 current = FrontierSave_GetStat(frontier, stat, index);
    if (current < value) {
        FrontierSave_SetStat(frontier, stat, index, value);
    } else if (current > 9999) {
        FrontierSave_SetStat(frontier, stat, index, 9999);
    }
}

u32 FrontierSpeciesRecords_sizeof(void) {
    return sizeof(struct FrontierSpeciesRecords);
}

void FrontierSpeciesRecords_Init(struct FrontierSpeciesRecords *data) {
    MI_CpuFill8(data, 0, sizeof(*data));
    data->unk_000 = -1;
}

u32 *FrontierSpeciesRecords_Load(SaveData *save, u32 heapId, u32 *out) {
    return (u32 *)LoadFrontierSpeciesRecords(save, (enum HeapID)heapId, (int *)out);
}

int FrontierSpeciesRecords_Save(SaveData *save, struct FrontierSpeciesRecords *data) {
    int result = SaveFrontierSpeciesRecords(save, data);
    return SaveGameNormal(save) | result;
}
typedef struct FrontierSpeciesRecords ExtraFrontier;
u32 FrontierSpeciesRecords_Set(ExtraFrontier *data, int type, int index, u16 value);
u32 FrontierSpeciesRecords_Raise(SaveData *save, u32 *data, u32 type, u32 index, u16 value);
BOOL FrontierSpeciesRecords_UpdateFromStat(SaveData *save, int stat, int indexStat, int slot, u32 type, enum HeapID heapId, u32 *loadResult, int *saveResult);

u32 FrontierSpeciesRecords_Get(SaveData *save, u32 *buffer, u32 type, u32 index) {
    ExtraFrontier *data = (ExtraFrontier *)buffer;
    if (!Save_CheckExtraChunksExist(save)) {
        return 0;
    }
    switch (type) {
    case 0:
        return data->stats[0][index];
    case 1:
        return data->stats[1][index];
    case 2:
        return data->stats[2][index];
    }
    GF_ASSERT(FALSE);
    return 0;
}

u32 FrontierSpeciesRecords_Set(ExtraFrontier *data, int type, int index, u16 value) {
    if (value > 9999) {
        value = 9999;
    }
    switch (type) {
    case 0:
        data->stats[0][index] = value;
        break;
    case 1:
        data->stats[1][index] = value;
        break;
    case 2:
        data->stats[2][index] = value;
        break;
    default:
        GF_ASSERT(FALSE);
        return 0;
    }
    return value;
}

u32 FrontierSpeciesRecords_Raise(SaveData *save, u32 *data, u32 type, u32 index, u16 value) {
    if (!Save_CheckExtraChunksExist(save)) {
        return 0;
    }
    u32 current = FrontierSpeciesRecords_Get(save, data, type, index);
    if (current < value) {
        return FrontierSpeciesRecords_Set((ExtraFrontier *)data, type, index, value);
    }
    if (current > 9999) {
        return FrontierSpeciesRecords_Set((ExtraFrontier *)data, type, index, 9999);
    }
    return current;
}

BOOL FrontierSpeciesRecords_UpdateFromStat(SaveData *save, int stat, int indexStat, int slot, u32 type, enum HeapID heapId, u32 *loadResult, int *saveResult) {
    FrontierSave *frontier;
    u16 value, index;
    BOOL changed = FALSE;
    GF_ASSERT(stat >= 34 && stat <= 60);
    GF_ASSERT(indexStat == 36 || indexStat == 48 || indexStat == 60);
    *loadResult = 1;
    *saveResult = 2;
    if (slot != 255) {
        return FALSE;
    }
    if (!Save_CheckExtraChunksExist(save)) {
        return FALSE;
    }
    frontier = Save_Frontier_GetStatic(save);
    value = FrontierSave_GetStat(frontier, stat, slot);
    index = FrontierSave_GetStat(frontier, indexStat, slot);
    u32 *data = FrontierSpeciesRecords_Load(save, heapId, loadResult);
    u32 old;
    if (*loadResult != 1) {
        old = 0;
    } else {
        old = FrontierSpeciesRecords_Get(save, data, type, index);
    }
    FrontierSpeciesRecords_Raise(save, data, type, index, value);
    if (value != old) {
        *saveResult = FrontierSpeciesRecords_Save(save, (struct FrontierSpeciesRecords *)data);
        changed = TRUE;
    }
    if (data != NULL) {
        Heap_Free(data);
    }
    return changed;
}

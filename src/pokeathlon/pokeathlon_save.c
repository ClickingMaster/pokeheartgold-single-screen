#include "pokeathlon/pokeathlon_save.h"

u32 PokeathlonSave_sizeof() {
    return sizeof(PokeathlonSave);
}

void PokeathlonSave_Init(PokeathlonSave *dest) {
    MI_CpuFill8(dest, 0, sizeof(PokeathlonSave));

    for (u8 i = 0; i < 10; i++) {
        for (u8 j = 0; j < 5; j++) {
            dest->recordsSolo[i].entries[j].score = 0xFFFF;
            dest->recordsLink[i].entries[j].score = 0xFFFF;
        }
    }
}

PokeathlonSave *Save_Pokeathlon_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_POKEATHLON);
}

PokeathlonSave *PokeathlonSave_dummy1(PokeathlonSave *pokeathlon) {
    return pokeathlon;
}

void *PokeathlonSave_GetMonMedals(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->prefix.legacy.bytes1F0;
}

Pokeathlon_RecordsSolo *PokeathlonSave_GetRecordsSolo(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->recordsSolo;
}

Pokeathlon_RecordsLink *PokeathlonSave_GetRecordsLink(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->recordsLink;
}

void *PokeathlonSave_GetSoloEventBestPoints(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->soloEventBestPoints;
}

PokeathlonStatistics *PokeathlonSave_GetStatistics(PokeathlonSave *pokeathlonSave) {
    return &pokeathlonSave->statistics;
}

PokeathlonSave *PokeathlonSave_dummy2(PokeathlonSave *pokeathlon) {
    return pokeathlon;
}

BOOL PokeathlonSave_MonHasMedal(PokeathlonSave *pokeathlonSave, int shift, int index) {
    if (shift < 0 || shift > 4) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    int indexMinusOne = index - 1;
    if (indexMinusOne < 0) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    if (!(((int)pokeathlonSave->prefix.legacy.bytes1F0[indexMinusOne]) >> shift & 1)) {
        return FALSE;
    }
    return TRUE;
}

Pokeathlon_RecordsSolo *PokeathlonSave_GetRecordsSolo2(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->recordsSolo;
}

Pokeathlon_RecordsLink *PokeathlonSave_GetRecordsLink2(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->recordsLink;
}

PokeathlonStatistics *PokeathlonSave_GetStatistics2(PokeathlonSave *pokeathlonSave) {
    return &pokeathlonSave->statistics;
}

void PokeathlonSave_AddMonMedals(PokeathlonSave *pokeathlonSave, int shift, int index) {
    u8 i, bit;
    int indexMinusOne = index - 1;
    if (indexMinusOne < 0) {
        GF_ASSERT(FALSE);
        return;
    }
    for (i = 0; i < 5; i++) {
        if ((u8)((shift >> i) & 1) != 0) {
            bit = 1 << i;
            pokeathlonSave->prefix.legacy.bytes1F0[indexMinusOne] |= bit;
        }
    }
}

void PokeathlonSave_AddAthletePoints(PokeathlonSave *pokeathlonSave, u16 amount) {
    pokeathlonSave->athletePoints += amount;
    if (pokeathlonSave->athletePoints > 99999) {
        pokeathlonSave->athletePoints = 99999;
    }
}

void PokeathlonSave_SubAthletePoints(PokeathlonSave *pokeathlonSave, u16 amount) {
    pokeathlonSave->athletePoints -= amount;
    if (pokeathlonSave->athletePoints < 0) {
        pokeathlonSave->athletePoints = 0;
    }
}

u32 PokeathlonSave_GetAthletePoints(PokeathlonSave *pokeathlonSave) {
    return pokeathlonSave->athletePoints;
}

BOOL PokeathlonSave_IsDataCardBought(PokeathlonSave *pokeathlon, int idx) {
    GF_ASSERT(idx < 27);
    return (pokeathlon->dataCardsBought >> idx) & 1;
}

void PokeathlonSave_SetDataCardBought(PokeathlonSave *pokeathlon, int idx) {
    GF_ASSERT(idx < 27);
    pokeathlon->dataCardsBought |= 1 << idx;
}

BOOL PokeathlonSave_IsDailyItemBought(PokeathlonSave *pokeathlon, int idx) {
    return (pokeathlon->dailyItemsBought >> idx) & 1;
}

void PokeathlonSave_SetDailyItemBought(PokeathlonSave *pokeathlon, int idx) {
    pokeathlon->dailyItemsBought |= 1 << idx;
}

void PokeathlonSave_ResetDailyItemsBought(PokeathlonSave *pokeathlon) {
    pokeathlon->dailyItemsBought = 0;
}

u32 PokeathlonSave_FriendshipRecords_sizeof() {
    return sizeof(PokeathlonSave_FriendshipRecords);
}

void PokeathlonSave_FriendshipRecords_Init(PokeathlonSave_FriendshipRecords *dest) {
    MI_CpuClear8(dest, sizeof(PokeathlonSave_FriendshipRecords));
}

PokeathlonSave_FriendshipRecords *Save_Pokeathlon_FriendshipRecords_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_POKEATHLON_FRIENDSHIP_RECORDS);
}

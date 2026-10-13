#include "pokewalker.h"

#include "global.h"

static BOOL pokewalkerHasBoxmon(POKEWALKER *pokeWalker);

POKEWALKER *Save_Pokewalker_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_POKEWALKER);
}

u32 Pokewalker_sizeof(void) {
    return sizeof(POKEWALKER);
}

void Pokewalker_Init(POKEWALKER *pokeWalker) {
    MI_CpuClearFast(pokeWalker, sizeof(POKEWALKER));
    Pokewalker_UnlockCourse(pokeWalker, WALKER_COURSE_REFRESHING_FIELD);
    Pokewalker_UnlockCourse(pokeWalker, WALKER_COURSE_NOISY_FOREST);
}

void Pokewalker_Reset(POKEWALKER *pokeWalker) {
    u32 bak = pokeWalker->unlockedCourses;
    Pokewalker_Init(pokeWalker);
    pokeWalker->unlockedCourses = bak;
}

u16 Pokewalker_GetLinkCount(const POKEWALKER *pokeWalker) {
    return pokeWalker->linkCount;
}

void Pokewalker_IncrementLinkCount(POKEWALKER *pokeWalker) {
    pokeWalker->linkCount++;
}

void Pokewalker_SetStepsAddWatts(POKEWALKER *pokeWalker, u32 a1, u32 a2) {
    pokeWalker->steps = a1;
    pokeWalker->watts += a2;
    if (pokeWalker->watts >= 9999999) {
        pokeWalker->watts = 9999999;
    }
}

void Pokewalker_GetStepsAndWatts(POKEWALKER *pokeWalker, u32 *a1, u32 *a2) {
    *a1 = pokeWalker->steps;
    *a2 = pokeWalker->watts;
}

void sub_02032688(POKEWALKER *pokeWalker, u16 *a1, u16 *a2) {
    GF_ASSERT(pokeWalker != NULL);
    *a1 = pokeWalker->unk_000;
    *a2 = pokeWalker->unk_002;
}

void sub_020326A4(POKEWALKER *pokeWalker, u16 a1, u16 a2) {
    GF_ASSERT(pokeWalker != NULL);
    pokeWalker->unk_000 = a1;
    pokeWalker->unk_002 = a2;
}

void Pokewalker_SetBoxMon(POKEWALKER *pokeWalker, BoxPokemon *boxMon) {
    MI_CpuCopyFast(boxMon, &pokeWalker->pokemon, sizeof(BoxPokemon));
}

void Pokewalker_ClearBoxMon(POKEWALKER *pokeWalker) {
    MI_CpuClearFast(&pokeWalker->pokemon, sizeof(BoxPokemon));
}

BOOL Pokewalker_TryGetBoxMon(POKEWALKER *pokeWalker, BoxPokemon *boxMon) {
    if (pokewalkerHasBoxmon(pokeWalker)) {
        MI_CpuCopyFast(&pokeWalker->pokemon, boxMon, sizeof(BoxPokemon));
        return TRUE;
    }
    return FALSE;
}

static BOOL pokewalkerHasBoxmon(POKEWALKER *pokeWalker) {
    // This is an annoying hack to get it matching.
    // Should just memcmp with (BoxPokemon){}
    u8 *ptr = (u8 *)pokeWalker;
    int i;

    for (i = 0; i < (int)sizeof(BoxPokemon); i++) {
        if (ptr[i + offsetof(POKEWALKER, pokemon)] != 0) {
            break;
        }
    }
    if (i == sizeof(BoxPokemon)) {
        return FALSE;
    }
    return TRUE;
}

u16 Pokewalker_GetCourse(POKEWALKER *pokeWalker) {
    return pokeWalker->course;
}

void Pokewalker_SetCourse(POKEWALKER *pokeWalker, u16 a1) {
    pokeWalker->course = a1;
}

void *Pokewalker_GetIrClock(POKEWALKER *pokeWalker) {
    return &pokeWalker->irClock;
}

BOOL Pokewalker_CourseIsUnlocked(POKEWALKER *pokeWalker, int courseNo) {
    if (courseNo >= 32) {
        return FALSE;
    }
    return (pokeWalker->unlockedCourses >> courseNo) & 1;
}

void Pokewalker_UnlockCourse(POKEWALKER *pokeWalker, int courseNo) {
    GF_ASSERT(courseNo < 32);
    pokeWalker->unlockedCourses |= (1 << courseNo);
}

u16 Pokewalker_HasReceivedStarfBerry(POKEWALKER *pokeWalker) {
    return pokeWalker->receivedStarfBerry;
}

void Pokewalker_SetReceivedStarfBerry(POKEWALKER *pokeWalker) {
    pokeWalker->receivedStarfBerry = TRUE;
}

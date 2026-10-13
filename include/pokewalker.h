#ifndef POKEHEARTGOLD_POKEWALKER_H
#define POKEHEARTGOLD_POKEWALKER_H

#include "constants/pokewalker_courses.h"

#include "pokemon.h"
#include "save.h"

typedef struct POKEWALKER {
    u16 unk_000;
    u16 unk_002;
    BoxPokemon pokemon;
    u8 filler_08C[0x6C];
    u16 course;
    u16 receivedStarfBerry;
    u32 irClock[10];
    u16 linkCount;
    u8 filler_126[2];
    u32 steps;
    u32 watts;
    u32 unlockedCourses;
} POKEWALKER; // size=0x134

void Pokewalker_UnlockCourse(POKEWALKER *pokeWalker, int courseNo);
POKEWALKER *Save_Pokewalker_Get(SaveData *saveData);
u32 Pokewalker_sizeof(void);
void Pokewalker_Init(POKEWALKER *pokeWalker);
void Pokewalker_Reset(POKEWALKER *pokeWalker);
u16 Pokewalker_GetLinkCount(const POKEWALKER *pokeWalker);
void Pokewalker_IncrementLinkCount(POKEWALKER *pokeWalker);
void Pokewalker_SetStepsAddWatts(POKEWALKER *pokeWalker, u32 a1, u32 a2);
void Pokewalker_GetStepsAndWatts(POKEWALKER *pokeWalker, u32 *a1, u32 *a2);
void sub_02032688(POKEWALKER *pokeWalker, u16 *a1, u16 *a2);
void sub_020326A4(POKEWALKER *pokeWalker, u16 a1, u16 a2);
void Pokewalker_SetBoxMon(POKEWALKER *pokeWalker, BoxPokemon *boxMon);
void Pokewalker_ClearBoxMon(POKEWALKER *pokeWalker);
BOOL Pokewalker_TryGetBoxMon(POKEWALKER *pokeWalker, BoxPokemon *boxMon);
u16 Pokewalker_GetCourse(POKEWALKER *pokeWalker);
void Pokewalker_SetCourse(POKEWALKER *pokeWalker, u16 a1);
void *Pokewalker_GetIrClock(POKEWALKER *pokeWalker);
BOOL Pokewalker_CourseIsUnlocked(POKEWALKER *pokeWalker, int courseNo);
u16 Pokewalker_HasReceivedStarfBerry(POKEWALKER *pokeWalker);
void Pokewalker_SetReceivedStarfBerry(POKEWALKER *pokeWalker);

#endif // POKEHEARTGOLD_POKEWALKER_H

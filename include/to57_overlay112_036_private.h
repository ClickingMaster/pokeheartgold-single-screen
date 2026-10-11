#ifndef TO57_R3_12C_PRIVATE_H
#define TO57_R3_12C_PRIVATE_H
#include "global.h"
typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;
#define W(p, o) (*(int *)((p)->raw + (o)))
#define P(p, o) (*(void **)((p)->raw + (o)))
typedef struct Ov112CourseEntry {
    u32 requirement;
    u8 raw04[0xbc];
} Ov112CourseEntry;
extern const u16 ov112_021F397E[][2];
extern const Ov112CourseEntry sPokewalkerCourses[];
int sub_02070438(u16, u8);
void sub_02032674(void *, u32 *, u32 *);
void *Save_Pokedex_Get(void *);
int Pokedex_GetNatDexFlag(void *);
int Pokewalker_CourseIsUnlocked(void *, int);
void Pokewalker_UnlockCourse(void *, int);
int ov112_021E9480(int, int, int);
void ov112_021E95A0(Ov112State *);
int ov112_021E9610(Ov112State *, int);
#endif

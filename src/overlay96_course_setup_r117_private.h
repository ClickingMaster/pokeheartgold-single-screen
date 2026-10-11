#ifndef POKEHEARTGOLD_OVERLAY96_COURSE_SETUP_R117_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY96_COURSE_SETUP_R117_PRIVATE_H

#include "global.h"

typedef struct Ov96R117ParticipantView {
    u16 species;
    u16 field2;
    u16 special;
    u8 kind;
    u8 field7;
    u32 participant;
    u32 fieldC;
} Ov96R117ParticipantView;

typedef struct Ov96R117Work {
    void *course;
    u8 pad04[8];
    void *spriteManager;
    u8 pad10[8];
    void *field18;
    void *objectManager;
} Ov96R117Work;

void *SpriteManager_GetSpriteList(void *);
void *PokeathlonCourseMonObjects_Create(void *, u32, u32, void *, void *);
void PokeathlonCourse_GetMonInfo(void *, int, int, Ov96R117ParticipantView *);
void *PokeathlonCourse_GetPerformance(void *, int, int);
int ov96_021E6108(const void *);
void PokeathlonCourseMonObjects_Load(void *, int, Ov96R117ParticipantView *, u32 *, u32, u32);

void ov96_02217AE4(Ov96R117Work *, void *);

#endif

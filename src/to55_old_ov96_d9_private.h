#ifndef TO52_R5_W02_PRIVATE_H
#define TO52_R5_W02_PRIVATE_H

#include <nitro/fx/fx_vec.h>

#include "global.h"

typedef struct Ov96Object {
    void *course;
    void *sprite;
    void *data;
    u32 state;
    VecFx32 previous;
    VecFx32 position;
    VecFx32 target;
    VecFx32 velocity;
    u32 flags;
    u8 field44;
    u8 row;
    u8 column;
    u8 field47;
} Ov96Object;

void ov96_021EB0A4(void *, s32, s32, s32 *, s32 *);
void ov96_0220D744(Ov96Object *);
BOOL ov96_0220D8C4(Ov96Object *, fx32, fx32);
void PokeathlonCourse_AddMonCounter(void *, u8, u8, u8, u32);
BOOL ov96_021EB0CC(void *, fx32, fx32, s32, s32);

extern const s16 ov96_0221CFBC[][3][2];
extern const s16 ov96_0221CFBE[][3][2];
extern const VecFx32 ov96_0221CEF4;

void ov96_0220D910(Ov96Object *);
void ov96_0220D9A4(Ov96Object *, void *);
int ov96_0220DAA0(Ov96Object *, s16, s16);

#endif

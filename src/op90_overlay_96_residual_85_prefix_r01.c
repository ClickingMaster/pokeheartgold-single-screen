#pragma force_active on
#include "global.h"
#include "math_util.h"
#include <nitro/fx/fx_vec.h>

typedef struct Player {
    u8 unk0[0x10];
    u32 group;
    u16 unk14;
    s16 unk16;
    u8 unk18;
    u8 unk19;
} Player;

typedef struct Ball {
    u8 unk0[0xC];
    u32 state;
    VecFx32 target;
    VecFx32 pos;
    u8 unk28[0x1C];
    u16 count;
    u16 level;
} Ball;

typedef struct Entry {
    Player *player;
    u16 index;
    u16 score;
} Entry;

typedef struct Work {
    Player *players[12];
    Ball *balls[12];
    Entry entries[12];
} Work;

extern VecFx32 ov96_0221CA30[];
u8 ov96_0220472C(u32 group, Entry **entries, Entry **sorted);
void ov96_02204320(int radius, VecFx32 *vec);
void ov96_022044A0(Work *work, u8 group, u8 idx, int power);

// @fn ov96_022044A0
void ov96_022044A0(Work *work, u8 group, u8 idx, int power) {
    int i;
    u8 n;
    u8 pick;
    f32 speed;
    Entry *entries[12];
    Entry *sorted[9];
    VecFx32 dir;
    VecFx32 *home;

    if (work->players[idx]->unk16 + LCRandom() % 51 <= 120 && work->balls[idx]->count < 4) {
        if (work->balls[idx]->state == 0) {
            work->balls[idx]->count = 1;
            work->balls[idx]->state = 2;
            work->balls[idx]->target = ov96_0221CA30[idx];
            return;
        }
        GF_ASSERT(work->balls[idx]->state == 2);
        work->balls[idx]->count++;
        if (work->balls[idx]->count <= 1) {
            work->balls[idx]->level = 0;
        } else if (work->balls[idx]->count <= 2) {
            work->balls[idx]->level = 1;
        } else if (work->balls[idx]->count <= 3) {
            work->balls[idx]->level = 2;
        } else {
            work->balls[idx]->level = 3;
        }
        return;
    }
    for (i = 0; i < 12; i++) {
        work->entries[i].player = work->players[i];
        work->entries[i].index = i;
        work->entries[i].score = work->players[i]->unk14 / 20 + LCRandom() % 21;
        if (work->players[i]->unk19 == 1) {
            work->entries[i].score++;
        }
        work->entries[i].score += (120 - work->players[i]->unk16) / 20;
    }
    for (i = 0; i < 12; i++) {
        entries[i] = &work->entries[i];
    }
    n = ov96_0220472C(group, entries, sorted);
    pick = LCRandom() % n;
    {
    VecFx32 zero = {0, 0, 0};
    home = &ov96_0221CA30[idx];
    VEC_Subtract(&ov96_0221CA30[(u8)sorted[pick]->index], home, &dir);
    VEC_Normalize(&dir, &dir);
    ov96_02204320(3, &dir);
    ov96_02204320((u8)work->players[idx]->group, &dir);
    speed = (f32)power / 40.0 + ((f32)work->players[idx]->unk16 / 50.0 + (f32)(LCRandom() % 51));
    if (speed > 2.0) {
        speed = 2.0f;
    } else if (speed < 1.0) {
        speed = 1.0f;
    }
    VEC_MultAdd((fx32)(4096.0f * speed), &dir, &zero, &work->balls[idx]->pos);
    work->balls[idx]->state = 1;
    work->balls[idx]->target = *home;
    }
}

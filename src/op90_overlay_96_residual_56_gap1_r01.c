#pragma force_active on
#include "global.h"

typedef struct Point2 {
    fx32 x;
    fx32 y;
} Point2;

typedef struct ActiveWord {
    u32 enabled;
} ActiveWord;

typedef struct GridEntry {
    int index;
    u32 unk04;
    u32 unk08;
    Point2 *coords;
    ActiveWord *position;
    void *unk14;
    VecFx32 *result;
    void *projection;
} GridEntry;

typedef struct Person {
    int enabled;
    u16 x;
    u16 y;
} Person;

typedef struct SearchCell {
    int x;
    int y;
    int px;
    int py;
    void *active;
    int unk14;
    int rank;
    int index;
} SearchCell;

typedef struct Work {
    GridEntry actors[12];
    SearchCell cells[25];
    int personIndex[4];
    u32 unk4B0;
    int attempts[12];
    u8 activeCount;
    u8 currentParticipant;
    u8 elapsed;
    u8 threshold;
    u8 selector;
    u8 slot;
    u8 phase;
} Work;

u16 LCRandom(void);
void ov96_021EAF78(void *projection, fx32 posX, fx32 posY, int *x, int *y, u32 *radius);
void ov96_021EB06C(void *projection, int x, int y, int *outX, int *outY);
void ov96_021F333C(int players, Person *persons);
void ov96_021F380C(u8 count, SearchCell **cells);
void ov96_021F3888(void *work, u8 count, Person *persons, GridEntry *target, SearchCell **cells);
void ov96_021EB0A4(void *projection, int x, int y, int *outX, int *outY);
int ov96_021F38FC(int x, int y);
int ov96_021F3B04(s32 x, s32 y, s16 *col, s16 *row);
u8 ov96_021F3B38(u32 count, SearchCell **cells, SearchCell **choices);
void ov96_021F350C(void *parent, u8 which, u8 players);

// @fn ov96_021F350C
void ov96_021F350C(void *parent, u8 which, u8 players) {
    SearchCell *all[25];
    SearchCell *choices[25];
    Person persons[4];
    SearchCell *near[9];
    SearchCell *far[16];
    int targetX, targetY;
    int x, y;
    u32 radius;
    int nearX, nearY;
    int farX, farY;
    s16 col;
    s16 row;
    Work *work = parent;
    GridEntry *target = &work->actors[which];
    int nearCount, farCount, i, count;
    SearchCell *choice;

    if (!target->position->enabled) {
        return;
    }
    if (!target->result) {
        GF_ASSERT(0);
        return;
    }
    if (work->attempts[work->selector] < 3 && (int)LCRandom() % 100 < 30) {
        work->attempts[work->selector]++;
        return;
    }
    for (i = 0; i < 12; i++) {
        GridEntry *entry = &work->actors[i];

        if (entry->position->enabled) {
            ov96_021EAF78(entry->projection, entry->coords->x, entry->coords->y, &x, &y, &radius);
            entry->index = ov96_021F38FC(x / FX32_ONE, y / FX32_ONE);
        } else {
            entry->index = -1;
        }
    }
    ov96_021EB06C(target->projection, target->coords->x / FX32_ONE, target->coords->y / FX32_ONE, &targetX, &targetY);
    target->index = ov96_021F38FC(targetX, targetY);
    ov96_021F333C(players, persons);
    for (i = 0; i < 4; i++) {
        if (persons[i].enabled > 0) {
            work->personIndex[i] = ov96_021F38FC(persons[i].x, persons[i].y);
        } else {
            work->personIndex[i] = -1;
        }
    }
    if (!ov96_021F3B04((u16)targetX, (u16)targetY, &col, &row)) {
        GF_ASSERT(0);
    }
    nearCount = 0;
    farCount = 0;
    for (i = 0; i < 25; i++) {
        int dx, dy;

        work->cells[i].index = i;
        work->cells[i].x = i % 5 - 2 + col;
        work->cells[i].y = i / 5 - 2 + row;
        if (work->cells[i].x < 0 || work->cells[i].y < 0 || work->cells[i].x >= 7 || work->cells[i].y >= 5) {
            work->cells[i].active = NULL;
        } else {
            work->cells[i].active = (void *)1;
        }
        dx = work->cells[i].x - col;
        dy = work->cells[i].y - row;
        all[i] = &work->cells[i];
        if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
            near[nearCount++] = &work->cells[i];
        } else {
            far[farCount++] = &work->cells[i];
        }
    }
    GF_ASSERT(nearCount == 9);
    GF_ASSERT(farCount == 16);
    ov96_021F380C(9, near);
    near[4]->px = targetX;
    near[4]->py = targetY;
    ov96_021F3888(work, 9, persons, target, near);
    count = ov96_021F3B38(9, near, choices);
    if (choices[0]->rank >= 3) {
        choice = choices[(u8)((int)LCRandom() % count)];
        ov96_021EB0A4(target->projection, choice->px, choice->py, &nearX, &nearY);
        target->result->x = (u32)nearX << FX32_SHIFT;
        target->result->y = (u32)nearY << FX32_SHIFT;
        target->result->z = 0;
        return;
    }
    ov96_021F380C(16, far);
    ov96_021F3888(work, 25, persons, target, all);
    count = ov96_021F3B38(25, all, choices);
    choice = choices[(u8)((int)LCRandom() % count)];
    ov96_021EB0A4(target->projection, choice->px, choice->py, &farX, &farY);
    target->result->x = (u32)farX << FX32_SHIFT;
    target->result->y = (u32)farY << FX32_SHIFT;
    target->result->z = 0;
}

#pragma force_active on
#include "global.h"
#include "math_util.h"

typedef struct Member {
    u8 unk00[8];
    f32 unk08;
    f32 fitness;
    u8 unk10[8];
    u8 mode;
    u8 unk19[3];
} Member; // size 0x1C

typedef struct Actor {
    void *sprites[3];
    u8 unk0C[0x0C];
    Member members[3];
    u8 unk6C[0x10];
    fx32 x;
    fx32 y;
    u8 unk84[7];
    u8 current;
    u8 unk8C[0x11];
    u8 blocked;
    u8 unk9E;
    u8 unk9F[0x0A];
    u8 cooldown;
    u8 moving;
    u8 unkAB[5];
    VecFx32 origin;
    VecFx32 target;
    u8 unkC8[8];
    u8 participant;
} Actor;

typedef struct Entry {
    Actor *actor;
    u32 unk04;
    int x;
    int y;
} Entry;

typedef struct Obstacle {
    int active;
    int x;
    int y;
    int type;
    int width;
} Obstacle;

typedef struct Goal {
    int x;
    int y;
    int randomX;
    int randomY;
    s16 score;
    s16 index;
} Goal;

typedef struct CycleWork {
    Entry entries[4];
    Obstacle obstacles[30];
    u8 lanes[4];
    Goal goals[16];
    u8 unk3DC[8];
    u8 excluded;
} CycleWork;

extern const s32 ov96_0221C770[16];

void ov96_021EB0A4(void *sprite, int x, int y, int *outX, int *outY);
void ov96_021EB03C(void *sprite, fx32 x, fx32 y, fx32 *outX, fx32 *outY);
void ov96_0220144C(int *value, int delta);
Goal *ov96_0220146C(CycleWork *work);
void PokeathlonCourse_AddMonCounter(void *course, u8 participant, u8 current, int a, int b);
void ov96_02200F84(void *course, CycleWork *work, u8 selected, s32 time);

// @fn ov96_02200F84
void ov96_02200F84(void *course, CycleWork *work, u8 selected, s32 time) {
    Obstacle obstacles[30];
    Entry entries[4];
    int scores[4];
    int sampledX, sampledY;
    fx32 convX, convY;
    int originX, originY;
    int threshold;
    int *laneScore;
    Entry *entry;
    int i, j, lane, base, start;
    Actor *actor;
    Goal *goal;
    u8 slot, next;
    int delta, column, row;

    actor = work->entries[selected].actor;
    if (actor->blocked || actor->members[actor->current].mode == 2 || actor->cooldown) {
        return;
    }
    for (i = 0; i < 4; i++) {
        scores[i] = 0;
    }
    threshold = 0;
    for (i = 0; i < 4; i++) {
        Actor *member;

        entries[i] = work->entries[i];
        member = entries[i].actor;
        ov96_021EB0A4(member->sprites[member->current], member->x / FX32_ONE, member->y / FX32_ONE, &sampledX, &sampledY);
        ov96_021EB03C(member->sprites[member->current], sampledX << 12, sampledY << 12, &convX, &convY);
        entries[i].x = convX / FX32_ONE;
        entries[i].y = convY / FX32_ONE;
        work->lanes[i] = (entries[i].y - 32) / 40;
        if (work->lanes[i] > 4) {
            GF_AssertFail();
        }
    }
    entry = &entries[selected];
    for (i = 0; i < 30; i++) {
        obstacles[i] = work->obstacles[i];
    }
    if (entry->x >= 864) {
        for (i = 0; i < 4; i++) {
            ov96_0220144C(&entries[i].x, -160);
        }
        for (i = 0; i < 30; i++) {
            ov96_0220144C(&obstacles[i].x, 80);
        }
    }
    for (i = 0; i < 16; i++) {
        work->goals[i].index = i;
        column = (i % 4) * 40;
        work->goals[i].x = entry->x + 20 + column;
        row = (i / 4) * 40;
        work->goals[i].y = row + 52;
        work->goals[i].randomX = entry->x + column + LCRandom() % 40;
        work->goals[i].randomY = row + 32 + LCRandom() % 40;
    }
    for (i = 0; i < 16; i++) {
        work->goals[i].score = ov96_0221C770[i];
    }
    for (lane = 0, laneScore = scores, base = 0, start = 32; lane < 4; lane++, laneScore++, base += 4, start += 40) {
        for (j = 0; j < 4; j++) {
            if (j == selected) {
                continue;
            }
            if (entry->x + 160 < entries[j].x) {
                continue;
            }
            delta = entries[j].x - entry->x;
            if (delta >= 0 && delta < 160) {
                if (lane == work->lanes[j]) {
                    if (lane == work->lanes[selected]) {
                        *laneScore -= 4;
                    } else {
                        *laneScore -= 2;
                    }
                }
                slot = delta / 40;
                if (slot >= 4) {
                    GF_AssertFail();
                } else {
                    work->goals[base + slot].score -= 3;
                }
            } else if (delta >= -80 && lane == work->lanes[j] && lane == work->lanes[selected]) {
                *laneScore += 2;
            }
        }
        for (j = 0; j < 30; j++) {
            if (!obstacles[j].active) {
                break;
            }
            if (entry->x + 160 >= obstacles[j].x) {
                delta = obstacles[j].x - entry->x;
                if (delta > 0 && delta <= 160 && start < obstacles[j].y + obstacles[j].width && obstacles[j].y < start + 40) {
                    if (obstacles[j].type == 3) {
                        scores[j] -= 1;
                    } else {
                        scores[j] -= 6;
                    }
                }
            }
        }
    }
    for (i = 0; i < 16; i++) {
        work->goals[i].score += scores[i / 4];
    }
    if (time < 2700) {
        for (i = 0; i < 4; i++) {
            work->goals[i * 4 + 2].score += 3;
            work->goals[i * 4 + 3].score += 3;
        }
    }
    if (entry->actor->members[entry->actor->current].fitness <= 20.0f) {
        for (i = 0; i < 4; i++) {
            work->goals[i * 4 + 2].score -= 2;
            work->goals[i * 4 + 3].score -= 2;
        }
        threshold += 8;
    }
    actor = entry->actor;
    next = (actor->current + 1) % 3;
    if (actor->members[next].mode == 1) {
        threshold -= 5;
    }
    goal = ov96_0220146C(work);
    if (goal->score >= threshold) {
        if (selected < work->excluded) {
            return;
        }
        ov96_021EB0A4(entry->actor->sprites[entry->actor->current], entry->x, entry->y, &originX, &originY);
        entry->actor->origin.x = originX << 12;
        entry->actor->origin.y = originY << 12;
        entry->actor->origin.z = 0;
        entry->actor->target.x = goal->randomX << 12;
        entry->actor->target.y = goal->randomY << 12;
        entry->actor->target.z = 0;
        entry->actor->moving = TRUE;
    } else {
        if (selected < work->excluded) {
            return;
        }
        entry->actor->blocked = TRUE;
        entry->actor->unk9E = TRUE;
        PokeathlonCourse_AddMonCounter(course, entry->actor->participant, entry->actor->current, 7, 1);
    }
}

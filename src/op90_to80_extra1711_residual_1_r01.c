#pragma force_active on
#include "global.h"
#include "pokeathlon/pokeathlon.h"
#include "sound_playback.h"

typedef struct Entry {
    void *obj;
    void *effect;
    void *trails[3];
    u8 unk14[0x4E];
    u8 trailIdx;
    u8 cooldown;
    u8 unk64[0x18];
} Entry; // size 0x7C

typedef struct Ball {
    void *obj;
    void *effect;
    u8 unk08[0x37];
    u8 cooldown;
    u8 unk40[4];
    u8 lastState;
    u8 unk45[7];
} Ball; // size 0x4C

typedef struct Work {
    u8 unk000[0x5C];
    Entry entries[4][3];
    Ball balls[2];
    u8 unk6C4[0x10];
    void *columns[4];
    void *others[4];
    u8 unk6F4[0x44];
    s32 score;
    u8 unk73C[0xC];
    void *loader;
    u8 unk74C[4];
    void *timer;
} Work;

typedef struct Packet {
    u8 x[12];
    u8 y[12];
    u8 ballX[2];
    u8 ballY[2];
    s32 states;
    s32 flags;
} Packet;

typedef struct Course {
    u8 unk0[7];
    u8 kind;
} Course;

void ov96_02214418(Work *);
void *ov96_021EAA20(void *);
void ov96_021E8BAC(void *);
void ov96_021EB52C(void *, int, int);
void ov96_021EB570(void *, int);
Course *ov96_021E60C0(PokeathlonCourseData *, int, int);
void ov96_021EB588(void *, VecFx32 *);
void *ov96_021EAA04(void *, u8);
void ov96_021EB06C(void *, s32, s32, s32 *, s32 *);
void ov96_021EB564(void *, int);
void ov96_021EAC5C(void *, int);
void ov96_021EAC0C(void *, u8);
void ov96_021EB01C(void *, int, int, int);
void ov96_022144C0(Entry *, u8 *);
void ov96_0221457C(Work *, Packet *, u8);
void ov96_022123B0(Work *, u8, int, u8, u8, u8);
s32 ov96_02213F5C(u8);
void ov96_02213FF4(Work *, u8, u8, u8);
void ov96_021EB630(void *, int);
BOOL ov96_021EB57C(void *);
void ov96_0221236C(PokeathlonCourseData *);
int ov96_02214904(void *);
void ov96_0221497C(void *, u8);
void ov96_0221490C(void *, s32);
void ov96_02214490(Work *);
void ov96_02211F38(PokeathlonCourseData *ctx);

static inline u8 GetKind(PokeathlonCourseData *ctx, int idx) {
    return ov96_021E60C0(ctx, idx / 3, idx % 3)->kind;
}

static inline void ClearVec(VecFx32 *vec) {
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
}

// @fn ov96_02211F38
void ov96_02211F38(PokeathlonCourseData *ctx) {
    u8 other;
    Work *work;
    int x, y;
    u8 state, flag;
    int trailIdx;
    int shift;
    u8 visible;
    int row;
    u8 team;
    Packet *packet;
    s32 i;
    Entry *entry;
    Ball *ball;
    void *obj;

    work = PokeathlonCourse_GetHeapAllocPtr4(ctx);
    team = ov96_021E5F24(ctx);
    packet = (Packet *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(ctx) + 0xF0);
    ov96_02214418(work);
    for (i = 0; i < 12; i++) {
        row = i / 3;
        entry = &work->entries[row][i % 3];
        ov96_021E8BAC(ov96_021EAA20(entry->obj));
        flag = (packet->flags >> (i * 2)) & 3;
        if (flag == 3) {
            ov96_021EB52C(entry->effect, 1, 1);
            ov96_021EB570(entry->effect, 12);
        } else if (flag == 2) {
            ov96_021EB52C(entry->effect, 1, 1);
            ov96_021EB570(entry->effect, 16);
            if (!IsSEPlaying(0x890)) {
                PlaySE(0x890);
            }
        } else {
            ov96_021EB52C(entry->effect, 1, 0);
        }
        x = packet->x[i];
        y = packet->y[i];
        state = (packet->states >> (i * 2)) & 3;
        {
        VecFx32 pos = {0, 0, 0};
        pos.x = x << 12;
        pos.y = y << 12;
        switch (GetKind(ctx, i)) {
        case 1:
            pos.y -= FX32_CONST(8);
            break;
        case 2:
            pos.y -= FX32_CONST(8);
            break;
        case 3:
            pos.y -= FX32_CONST(16);
            break;
        default:
            GF_AssertFail();
            break;
        }
        ov96_021EB588(entry->effect, &pos);
        }
        if (flag == 1) {
            if (entry->cooldown == 0) {
                VecFx32 trailPos = {0, 0, 0};
                s32 outX, outY;

                obj = ov96_021EAA04(work->loader, i);
                trailIdx = entry->trailIdx;
                ov96_021EB06C(obj, packet->x[i], packet->y[i], &outX, &outY);
                trailPos.x = outX << 12;
                trailPos.y = outY << 12;
                ov96_021EB588(entry->trails[trailIdx], &trailPos);
                ov96_021EB52C(entry->trails[trailIdx], 1, 1);
                ov96_021EB564(entry->trails[trailIdx], 14);
                entry->trailIdx = (entry->trailIdx + 1) % 3;
                entry->cooldown = 5;
                if (row == team) {
                    PlaySE(0x8CC);
                }
            } else {
                entry->cooldown--;
            }
        }
        obj = ov96_021EAA04(work->loader, i);
        if (flag == 2) {
            ov96_021EAC5C(obj, 20);
        } else {
            ov96_021EAC0C(obj, state + 1);
        }
        ov96_021EB01C(obj, x, y, 1);
    }

    ov96_022144C0(work->entries[team], &packet->y[team * 3]);
    ov96_0221457C(work, packet, team);
    visible = (packet->states >> 30) & 1;
    other = (packet->states >> 31) & 1;
    for (i = 0, shift = 0; i < 2; i++, shift += 2) {
        VecFx32 ballPos = {0, 0, 0};
        ball = &work->balls[i];
        state = ((u8)(packet->states >> 24) >> shift) & 3;
        if (state == 1) {
            ov96_021EB52C(ball->obj, 1, 0);
            ov96_021EB52C(ball->effect, 1, 0);
            ov96_022123B0(work, i, (i == 0 && visible) ? 1 : 0, packet->ballX[i], packet->ballY[i], team);
        } else {
            ball->cooldown = 0;
            if (i == 1) {
                if (other) {
                    ov96_021EB52C(ball->obj, 1, 1);
                } else {
                    ov96_021EB52C(ball->obj, 1, 0);
                }
            } else {
                ov96_021EB52C(ball->obj, 1, 1);
            }
            if (state == 2) {
                ov96_021EB52C(ball->effect, 1, 1);
                if (ball->lastState != 2) {
                    PlaySE(0x8C7);
                }
            } else {
                ov96_021EB52C(ball->effect, 1, 0);
            }
            if (i == 0 && visible) {
                ov96_021EB570(ball->obj, 11);
            } else {
                ov96_021EB570(ball->obj, 10);
            }
        }
        ball->lastState = state;
        ballPos.x = packet->ballX[i] << 12;
        ballPos.y = ov96_02213F5C(packet->ballY[i]);
        ov96_02213FF4(work, i, visible, packet->ballY[i]);
        ov96_021EB588(ball->obj, &ballPos);
        ov96_021EB588(ball->effect, &ballPos);
        if (packet->ballY[i] < 192) {
            ov96_021EB630(ball->obj, 46);
        } else {
            ov96_021EB630(ball->obj, 4);
        }
    }

    for (i = 0; i < 4; i++) {
        if (!ov96_021EB57C(work->columns[i])) {
            ov96_021EB52C(work->others[i], 1, 0);
        }
    }
    ov96_0221236C(ctx);
    ov96_0221497C(work->timer, ov96_02214904(work->timer));
    ov96_0221490C(work->timer, work->score);
    ov96_02214490(work);
}

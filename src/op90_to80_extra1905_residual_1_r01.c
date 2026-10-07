#pragma force_active on
#include "global.h"
#include <nitro/fx/fx_vec.h>

typedef struct Member {
    void *sprite;
    u8 unk4[0x1C];
    int unk20;
    VecFx32 target;
    VecFx32 pos;
    VecFx32 dir;
    int state;
    VecFx32 vel;
    s16 unk58;
    s16 unk5A;
    u16 facing;
    u8 unk5E[2];
    u8 unk60;
    u8 unk61[3];
    fx32 unk64;
    fx32 unk68;
    u16 angle;
    u8 unk6E[2];
    u8 unk70;
    u8 power;
    u8 unk72;
    u8 unk73;
    u8 unk74;
    u8 unk75;
    u8 unk76[2];
    int mode;
} Member;

typedef struct Team {
    Member members[3];
} Team;

typedef struct Ball {
    u8 unk0[8];
    VecFx32 pos;
    VecFx32 dir;
    VecFx32 dir2;
    u8 hit[12];
    u8 kind;
    u8 state;
    u8 unk3A;
    u8 lastHitter;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42[3];
    u8 unk45;
    u16 unk46;
    int unk48;
} Ball;

typedef struct Work {
    u8 unk0[0x5C];
    Team teams[4];
    Ball balls[2];
} Work;

extern u8 ov96_0221D514[][30];
extern u8 ov96_0221D5AA[][30];
extern VecFx32 ov96_0221D4B4[4][2];

Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void ov96_021EAF78(void *sprite, fx32 x, fx32 y, fx32 *outX, fx32 *outY, int *outH);
BOOL ov96_0221341C(VecFx32 *a, fx32 ra, VecFx32 *b, fx32 rb);
BOOL ov96_02213534(VecFx32 *a, VecFx32 *b, u16 angle);
void ov96_022134D4(Work *work, Ball *ball, VecFx32 *dir);
void ov96_021E8228(void *course, u8 team, u8 member, int event, int value);
void ov96_022127F4(void *course);

// @fn ov96_022127F4
void ov96_022127F4(void *course) {
    int slot;
    VecFx32 *pos;
    Team *teams;
    int k;
    Work *work;
    int radius;
    u8 val;
    int team;
    fx32 rad;
    Ball *ball;
    Member *m;
    int i;
    fx32 dot;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    for (k = 0, ball = work->balls; k < 2; k++, ball++) {
        if (ball->kind == 0 || (ball->state != 0 && ball->state != 2)) {
            continue;
        }
        if (ball->kind == 1) {
            radius = 8;
        } else if (ball->kind == 2) {
            radius = 12;
        }
        rad = radius * FX32_ONE;
        for (i = 0, pos = &ball->pos, teams = work->teams; i < 12; i++) {
            VecFx32 v = {0, 0, 0};
            VecFx32 d;
            int h;
            slot = i % 3;
            team = i / 3;
            m = &teams[team].members[slot];
            ov96_021EAF78(m->sprite, m->pos.x, m->pos.y, &v.x, &v.y, &h);
            if (ov96_0221341C(&v, h * FX32_ONE, pos, rad)) {
                if ((ball->hit[i] != 0 && ball->unk48 != 0) || m->state == 3 || m->state == 2) {
                    continue;
                }
                VEC_Subtract(pos, &v, &d);
                VEC_Normalize(&d, &d);
                if (ball->state == 2) {
                    int power;
                    if (ball->lastHitter == i) {
                        continue;
                    }
                    power = m->power - ball->unk45;
                    if (power <= 0) {
                        m->power = 0;
                        m->mode = 2;
                        m->unk60 = 90;
                        m->state = 3;
                        m->unk5A = 30;
                        m->vel = d;
                        m->vel.x *= -1;
                        m->vel.y *= -1;
                        m->unk58 = 7;
                        ov96_021E8228(course, team, slot, 1, 1);
                    } else {
                        m->power = power;
                        m->state = 3;
                        m->unk5A = 30;
                        m->unk58 = 7;
                        m->vel = d;
                        m->vel.x *= -1;
                        m->vel.y *= -1;
                    }
                } else {
                    if (VEC_Mag(&m->dir)) {
                        BOOL res = ov96_02213534(&m->dir, &d, (m->angle << 16) / 360);
                        if (m->mode == 1 && res) {
                            ball->state = 2;
                            ball->unk3E = 2;
                            ball->unk41 = m->unk73;
                            ball->unk45 = m->unk72;
                            ball->unk46 = m->unk75;
                            val = ov96_0221D514[ball->unk41][0];
                            m->state = 1;
                            m->vel = d;
                            m->vel.x *= -1;
                            m->vel.y *= -1;
                            m->unk58 = 3;
                            m->mode = 0;
                            ball->unk3C = 1;
                            ball->unk3D = 30;
                            ov96_021E8228(course, team, slot, 4, 1);
                        } else {
                            ball->unk3E = 1;
                            ball->unk41 = m->unk73;
                            val = ov96_0221D5AA[ball->unk41][0];
                            ball->unk3C = 1;
                            ball->unk3D = 30;
                        }
                        ball->unk40 = val;
                    }
                    ov96_022134D4(work, ball, &d);
                    ball->lastHitter = i;
                }
                ball->hit[i] = 1;
            } else {
                ball->hit[i] = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            VecFx32 n;
            VecFx32 *wall = &ov96_0221D4B4[i % 4][i / 4];
            if (ov96_0221341C(wall, 2 * FX32_ONE * 4, &ball->pos, rad)) {
                VEC_Subtract(wall, &ball->pos, &n);
                VEC_Normalize(&n, &n);
                dot = VEC_DotProduct(&ball->dir, &n);
                if (dot > 0) {
                    VEC_MultAdd(-(dot * 2), &n, &ball->dir, &ball->dir);
                    VEC_Normalize(&ball->dir, &ball->dir2);
                }
                break;
            }
        }
    }
}

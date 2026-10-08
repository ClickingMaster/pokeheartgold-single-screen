#pragma force_active on
#include "global.h"

typedef struct Member {
    u8 pad0[0x20];
    int speedArg;
    VecFx32 target;
    VecFx32 pos;
    VecFx32 dir;
    int state;
    VecFx32 vel;
    s16 moveTimer;
    s16 waitTimer;
    u16 facing;
    u8 pad5E[2];
    u8 stunTimer;
    u8 pad61[3];
    fx32 speed;
    fx32 fastSpeed;
    u8 pad6C[4];
    u8 savedA;
    u8 savedB;
    u8 pad72[6];
    int mode;
} Member;

typedef struct Team {
    Member members[3];
} Team;

typedef struct Work {
    u8 pad0[0x5c];
    Team teams[4];
} Work;

extern const VecFx32 ov96_0221D25C;

Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void ov96_02213FB4(Member *m);
void ov96_02213558(Member *m);
u16 CalcAngleBetweenVecs(VecFx32 *a, VecFx32 *b);
BOOL ov96_02213728(VecFx32 *pos, VecFx32 *prev, int arg, int flag, VecFx32 *out);
void ov96_022124F8(void *course);

// @fn ov96_022124F8
void ov96_022124F8(void *course) {
    VecFx32 prev[12];
    Work *w;
    int i;
    Member *m;
    fx32 dist;
    fx32 limit;
    u16 angle;

    w = PokeathlonCourse_GetHeapAllocPtr4(course);
    VecFx32 zero = { 0, 0, 0 };
    VecFx32 diff;
    VecFx32 dir;
    VecFx32 cur;

    for (i = 0; i < 12; i++) {
        m = &w->teams[i / 3].members[i % 3];
        m->dir = zero;
        prev[i] = m->pos;
        cur = m->pos;
        if (m->state != 0) {
            if ((m->state == 2 || m->state == 3) && m->waitTimer > 0) {
                m->waitTimer--;
            }
            if (m->moveTimer == 0 && m->waitTimer <= 0) {
                m->state = 0;
                m->waitTimer = 0;
            } else {
                VEC_MultAdd(m->moveTimer << 12, &m->vel, &cur, &cur);
                m->pos = cur;
                m->target = m->pos;
                if (m->moveTimer > 0) {
                    m->moveTimer--;
                }
                if (m->mode != 2 && m->state == 3) {
                    ov96_02213FB4(m);
                }
            }
        }
        if (m->mode == 2 || m->stunTimer != 0) {
            m->stunTimer--;
            if (m->stunTimer == 0) {
                m->mode = 0;
                m->savedB = m->savedA;
            }
            continue;
        }
        if (m->state != 0) {
            continue;
        }
        VEC_Subtract(&m->target, &cur, &diff);
        dist = VEC_Mag(&diff);
        if (dist <= 0) {
            ov96_02213558(m);
            continue;
        }
        VEC_Normalize(&diff, &dir);
        m->dir = dir;
        if (m->mode == 1) {
            limit = m->fastSpeed;
        } else if (m->mode == 3) {
            limit = 2 * FX32_ONE;
        } else {
            limit = m->speed;
        }
        if (dist <= limit) {
            cur = m->target;
        } else {
            VecFx32 zero2 = { 0, 0, 0 };

            VEC_MultAdd(limit, &dir, &zero2, &diff);
            VEC_Add(&diff, &cur, &cur);
        }
        if (dist != 0) {
            VecFx32 a = ov96_0221D25C;
            VecFx32 b;

            b.x = dir.y;
            b.z = dir.x;
            b.y = 0;
            angle = CalcAngleBetweenVecs(&a, &b);
            if (angle <= 0x2000 || angle >= 0xe000) {
                m->facing = 4;
            } else if (angle > 0x2000 && angle < 0x6000) {
                m->facing = 2;
            } else if (angle >= 0x6000 && angle <= 0xa000) {
                m->facing = 3;
            } else {
                m->facing = 1;
            }
        }
        m->pos = cur;
    }
    {
    Member *m2;
    VecFx32 *p;
    for (i = 0, p = prev; i < 12; i++, p++) {
        m2 = &w->teams[i / 3].members[i % 3];
        if (m2->pos.x != p->x || m2->pos.y != p->y) {
            VecFx32 out;

            if (ov96_02213728(&m2->pos, &prev[i], m2->speedArg, 1, &out)) {
                m2->pos = out;
                m2->target = m2->pos;
            }
        }
        if (m2->pos.x == m2->target.x && m2->pos.y == m2->target.y && m2->mode != 3 && m2->mode != 2) {
            m2->mode = 0;
        }
    }
    }
}

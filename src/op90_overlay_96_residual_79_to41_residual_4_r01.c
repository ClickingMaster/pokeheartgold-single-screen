#pragma force_active on
#include "global.h"

typedef struct Rec {
    u8 pad0[8];
    f32 power;
    int timer;
    u8 pad10[7];
    u8 kind;
    u8 state;
    u8 pad19[3];
} Rec;

typedef struct Player {
    void *sprites[4];
    u8 pad10[8];
    Rec recs[3];
    u8 pad6C[0x10];
    int posX;
    int posY;
    u8 pad84[7];
    u8 idx;
    VecFx32 vel;
    u8 hit[4];
    u8 pad9C;
    u8 out;
    u8 pad9E[6];
    u8 lastKind;
    u8 padA5[4];
    u8 stun;
    u8 padAA[0x26];
    u8 id;
    u8 padD1[3];
} Player;

void ov96_021EB0A4(void *sprite, int x, int y, int *outX, int *outY);
void ov96_021EAF78(void *sprite, fx32 x, fx32 y, fx32 *outX, fx32 *outY, int *radius);
void PokeathlonCourse_AddMonCounter(void *course, u8 player, u8 type, u8 index, u32 value);
BOOL ov96_021FF764(void *course, Player *players, VecFx32 *out);

// @fn ov96_021FF764
BOOL ov96_021FF764(void *course, Player *players, VecFx32 *out) {
    VecFx32 posP;
    VecFx32 posQ;
    VecFx32 d;
    int radP, radQ;
    int px, py, qx, qy;
    void *spriteP, *spriteQ;
    BOOL result;
    u8 kIdx;
    int pRaw;
    int rawX;
    s32 i;
    u8 pIdx;
    int j;
    Player *p, *q;

    result = FALSE;
    posP.z = 0;
    posQ.z = 0;
    for (i = 0, p = players; i < 4; i++, p++) {
        pIdx = p->idx;
        if (p->out != 0 || p->stun != 0) {
            continue;
        }
        spriteP = p->sprites[pIdx];
        ov96_021EB0A4(spriteP, p->posX / FX32_ONE, p->posY / FX32_ONE, &px, &py);
        for (j = 0, q = players; j < 4; j++, q++) {
            if (i == j) {
                continue;
            }
            if (q->out != 0) {
                continue;
            }
            rawX = px;
            spriteQ = q->sprites[q->idx];
            ov96_021EB0A4(spriteQ, q->posX / FX32_ONE, q->posY / FX32_ONE, &qx, &qy);
            {
                pRaw = p->posX;

                if (pRaw > 768.0 * FX32_ONE && q->posX < 256 * FX32_ONE) {
                    {
                    double t = qx;

                    t += 1024.0;
                    qx = t;
                }
                } else if (q->posX > 768.0 * FX32_ONE && pRaw < 256 * FX32_ONE) {
                    {
                    double t = rawX;

                    t += 1024.0;
                    rawX = t;
                }
                }
            }
            ov96_021EAF78(spriteQ, qx << 12, qy << 12, &posQ.x, &posQ.y, &radQ);
            ov96_021EAF78(spriteP, rawX << 12, py << 12, &posP.x, &posP.y, &radP);
            VEC_Subtract(&posQ, &posP, &d);
            if (VEC_Mag(&d) < (radQ + radP) << 12) {
                if (p->hit[j] != 0) {
                    continue;
                }
                VecFx32 zero = { 0, 0, 0 };

                if (VEC_DotProduct(&d, &p->vel) <= 0) {
                    continue;
                }
                if (VEC_Mag(&p->vel) != 0) {
                    kIdx = p->idx;
                    VEC_MultAdd(4096.0f * p->recs[kIdx].power, &p->vel, &zero, &q->vel);
                    if (VEC_Mag(&q->vel) > 11 * FX32_ONE) {
                        VecFx32 zero2 = { 0, 0, 0 };

                        VEC_Normalize(&q->vel, &q->vel);
                        VEC_MultAdd(11 * FX32_ONE, &q->vel, &zero2, &q->vel);
                    }
                }
                if (q->recs[pIdx].state == 1) {
                    q->recs[pIdx].timer = 0;
                    q->recs[pIdx].state = 2;
                    q->lastKind = q->recs[pIdx].kind;
                    PokeathlonCourse_AddMonCounter(course, q->id, q->idx, 1, 1);
                }
                {
                    VecFx32 stop = { 0, 0, 0 };

                    p->vel = stop;
                }
                if (p->recs[kIdx].state == 1) {
                    p->recs[kIdx].timer = 0;
                    p->recs[kIdx].state = 2;
                    p->lastKind = p->recs[kIdx].kind;
                    PokeathlonCourse_AddMonCounter(course, p->id, p->idx, 1, 1);
                }
                GF_ASSERT(p->hit[j] == 0);
                GF_ASSERT(q->hit[i] == 0);
                q->hit[j] = 1;
                p->hit[i] = 1;
                if (!result) {
                    VecFx32 n = { 0, 0, 0 };

                    result = TRUE;
                    VEC_Subtract(&posQ, &posP, &n);
                    VEC_Normalize(&n, &n);
                    VEC_MultAdd(radP << 12, &n, &posP, &n);
                    if (n.x >= 1024.0 * FX32_ONE) {
                        n.x = n.x - 1024.0 * FX32_ONE;
                    }
                    *out = n;
                }
                q->stun = 6;
                p->stun = 18;
                PokeathlonCourse_AddMonCounter(course, q->id, q->idx, 4, 1);
                PokeathlonCourse_AddMonCounter(course, p->id, p->idx, 4, 1);
                break;
            } else {
                p->hit[j] = 0;
                q->hit[i] = 0;
            }
        }
    }
    return result;
}

#pragma force_active on
#include "global.h"

typedef struct Particle {
    u32 unk0;
    int state;
    VecFx32 pos;
    VecFx32 vel;
    VecFx32 accel;
    u8 pad28[0xc];
    u8 counter;
    u8 pad35[2];
    u8 hit;
    u16 unk38;
    u16 kind;
    u8 pad40[8];
} Particle;

typedef struct Player {
    f32 scale;
    f32 power;
    u8 pad8[4];
    u16 hits;
    s16 energy;
    u8 meter;
    u8 pad11[2];
    u8 active;
    u8 pad14[0xc];
} Player;

typedef struct Work {
    u8 pad0[0xc0];
    Particle particles[12];
    Player players[12];
} Work;

extern VecFx32 ov96_0221C98C[];

void VEC_Subtract(const VecFx32 *a, const VecFx32 *b, VecFx32 *dest);
fx32 VEC_Mag(VecFx32 *v);
void PokeathlonCourse_AddMonCounter(void *course, u8 a, u8 b, int c, int d);
void ov96_02203544(void *course, Work *w);

// @fn ov96_02203544
void ov96_02203544(void *course, Work *w) {
    int j, i;
    int group;
    f32 factor;
    int damage;
    int mag;
    u8 gain;
    VecFx32 diff;

    for (j = 0; j < 12; j++) {
        w->players[j].active = 0;
        for (i = 0; i < 12; i++) {
            if (w->particles[i].state != 1) {
                continue;
            }
            group = i / 3;
            if (group == j / 3) {
                continue;
            }
            VEC_Subtract(&ov96_0221C98C[j], &w->particles[i].pos, &diff);
            if (VEC_Mag(&diff) >= 3 * FX32_ONE) {
                continue;
            }
            w->particles[i].state = 3;
            PokeathlonCourse_AddMonCounter(course, group, i % 3, 3, 1);
            w->players[i].hits++;
            if (w->players[i].hits > 999) {
                w->players[i].hits = 999;
            }
            w->particles[i].hit = 1;
            w->players[j].active = 1;
            switch (w->particles[i].kind) {
            case 0:
                factor = 1.0f;
                break;
            case 1:
                factor = 20.0f;
                break;
            case 2:
                factor = 50.0f;
                break;
            case 3:
                factor = 100.0f;
                break;
            default:
                GF_ASSERT(FALSE);
                factor = 1.0f;
                break;
            }
            w->particles[i].kind = 0;
            w->particles[i].unk38 = 0;
            mag = VEC_Mag(&w->particles[i].vel) / FX32_ONE;
            damage = (f32)mag * factor * w->players[i].scale * w->players[j].power;
            GF_ASSERT(damage >= 0);
            w->players[j].energy = w->players[j].energy - damage - 30;
            if (w->players[j].energy < 0) {
                w->players[j].energy = 0;
            }
            gain = 120 - w->players[j].energy;
            if (w->players[j].meter == 0) {
                if (gain > 40) {
                    w->players[j].meter = gain;
                    PokeathlonCourse_AddMonCounter(course, j / 3, j % 3, 1, 1);
                }
            } else {
                w->players[j].meter += gain;
                PokeathlonCourse_AddMonCounter(course, j / 3, j % 3, 1, 1);
            }
        }
    }
}

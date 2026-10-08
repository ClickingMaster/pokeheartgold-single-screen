#pragma force_active on
#include "global.h"

typedef struct Member {
    u8 unk00[8];
    f32 energy;
    f32 maximum;
    u8 speed;
    u8 unk11;
    u8 limit;
    u8 cooldown;
    u8 mode;
    u8 unk15[7];
} Member;

typedef struct Actor {
    void *sprites[3];
    s32 progress[3];
    u8 unk18[4];
    Member members[3];
    u8 unk70[0x7C - 0x70];
    VecFx32 position;
    u8 unk88[3];
    u8 current;
    VecFx32 velocity;
    u8 unk98[4];
    u8 laps;
    u8 disabled;
    u8 unk9E[4];
    u8 grade;
    u8 tired;
    u8 cooldown;
    u8 collision;
    u8 unkA6[2];
    u8 best;
    u8 reset;
    u8 unkAA[0xCC - 0xAA];
    s32 state;
    u8 owner;
    u8 wrapped;
    u8 unkD2[2];
} Actor;

typedef struct Work {
    u8 unk000[0x30];
    Actor actors[4];
    u8 unk380[0x3C2 - 0x380];
    u8 boost;
    u8 unk3C3[0x63C - 0x3C3];
    u8 hitFlag;
    u8 hitY;
    u16 hitX;
} Work;

void *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void ov96_021FE538(void *course, Work *work, Actor *actor);
int ov96_021EAF8C(void *sprite);
int ov96_021FF5A8(Work *work, int state, int a2, VecFx32 *prev, VecFx32 *pos, VecFx32 *out);
BOOL ov96_021FF764(void *course, Actor *actors, VecFx32 *hit);
void ov96_021E8228(void *course, u8 owner, u8 current, u32 event, u32 value);
void ov96_021FDE7C(void *course);

// @fn ov96_021FDE7C
void ov96_021FDE7C(void *course) {
    VecFx32 hit;
    VecFx32 prev;
    VecFx32 vel;
    u8 skip[4];
    Work *work;
    int i;
    int j;
    Actor *actor;
    int result;
    int hi;
    int old;
    fx32 mag;
    fx32 target;
    fx32 speed;
    f32 coefficient;
    f32 energy;
    u8 limit;

    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    work->hitFlag = 0;
    for (i = 0; i < 4; i++) {
        skip[i] = 0;
        if (work->actors[i].reset) {
            ov96_021FE538(course, work, &work->actors[i]);
            skip[i] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (skip[i]) {
            continue;
        }
        actor = &work->actors[i];
        prev = actor->position;
        VEC_Add(&actor->position, &actor->velocity, &actor->position);
        actor->progress[actor->current] += actor->velocity.x;
        if (actor->position.y < FX32_CONST(32)) {
            actor->position.y = FX32_CONST(32);
        } else if (actor->position.y > FX32_CONST(168)) {
            actor->position.y = FX32_CONST(168);
        }
        result = ov96_021FF5A8(work, actor->state, ov96_021EAF8C(actor->sprites[actor->current]), &prev, &actor->position, &actor->position);
        hi = (u8)(result >> 8);
        result = (u8)result;
        if (hi == 1) {
            actor->collision = TRUE;
        } else {
            actor->collision = FALSE;
        }
        actor->state = result;
        switch (result) {
        case 2:
            if (actor->velocity.y > 0) {
                actor->velocity.y *= -1;
            }
            break;
        case 3:
            if (actor->velocity.x < 0) {
                actor->velocity.x *= -1;
            }
            break;
        case 4:
            if (actor->velocity.y < 0) {
                actor->velocity.y *= -1;
            }
            break;
        case 5:
            if (actor->velocity.x > 0) {
                actor->velocity.x *= -1;
            }
            break;
        case 6:
            if (actor->velocity.x > 0) {
                actor->velocity.x *= -1;
            }
            if (actor->velocity.y > 0) {
                actor->velocity.y *= -1;
            }
            break;
        case 7:
            if (actor->velocity.x < 0) {
                actor->velocity.x *= -1;
            }
            if (actor->velocity.y > 0) {
                actor->velocity.y *= -1;
            }
            break;
        case 8:
            if (actor->velocity.x < 0) {
                actor->velocity.x *= -1;
            }
            if (actor->velocity.y < 0) {
                actor->velocity.y *= -1;
            }
            break;
        case 9:
            if (actor->velocity.x > 0) {
                actor->velocity.x *= -1;
            }
            if (actor->velocity.y < 0) {
                actor->velocity.y *= -1;
            }
            break;
        }
        result = ov96_021FF764(course, work->actors, &hit);
        if (work->hitFlag == 0 && result) {
            work->hitFlag = 1;
            work->hitX = hit.x / FX32_ONE;
            work->hitY = hit.y / FX32_ONE;
        }
        old = actor->position.x;
        if (old >= 4194304.0) {
            actor->position.x = old - 4194304.0;
            if (!actor->wrapped && actor->laps < 60) {
                actor->laps++;
            }
            if (actor->best < actor->laps) {
                actor->best = actor->laps;
            }
            actor->wrapped = FALSE;
        } else if (old < 0) {
            actor->position.x = 4194304.0 + old;
            if (actor->laps != 0) {
                actor->laps--;
            } else if (actor->laps == 0) {
                actor->wrapped = TRUE;
            }
        }
        vel = actor->velocity;
        mag = VEC_Mag(&vel);
        if (!actor->disabled && actor->collision) {
            target = actor->members[actor->current].speed << FX32_SHIFT;
            if (mag > target) {
                VecFx32 zero = { 0, 0, 0 };
                VecFx32 norm;

                norm = actor->velocity;
                VEC_Normalize(&norm, &norm);
                VEC_MultAdd(target, &norm, &zero, &norm);
                actor->velocity = norm;
                vel = actor->velocity;
                mag = VEC_Mag(&vel);
            }
        }
        if (mag > 0) {
            coefficient = 1.0f;
            speed = 4096.0 * (0.4 * coefficient);
            if (work->boost) {
                speed += FX32_CONST(3);
            }
            VEC_Normalize(&vel, &vel);
            vel.x = FX_Mul(vel.x, speed);
            vel.y = FX_Mul(vel.y, speed);
            target = VEC_Mag(&vel);
            if (VEC_Mag(&actor->velocity) >= target) {
                VEC_Subtract(&actor->velocity, &vel, &actor->velocity);
            } else {
                actor->velocity.x = 0;
                actor->velocity.y = 0;
            }
        }
        mag = VEC_Mag(&actor->velocity);
        limit = actor->members[actor->current].limit;
        if (mag >= (limit - 1) << FX32_SHIFT) {
            actor->grade = 3;
        } else if (mag >= (limit - 3) << FX32_SHIFT) {
            actor->grade = 2;
        } else if (mag == 0) {
            actor->grade = 0;
        } else {
            actor->grade = 1;
        }
        if (actor->disabled) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            energy = actor->members[j].energy;
            if (j == actor->current) {
                if (actor->members[j].mode != 2) {
                    switch (actor->grade) {
                    case 0:
                        energy += 0.5;
                        break;
                    case 1:
                    case 2:
                    case 3:
                        energy = energy - 0.5;
                        break;
                    }
                    if (actor->tired) {
                        if (energy > 40.0) {
                            energy = 40.0f;
                        }
                    } else if (energy > actor->members[j].maximum) {
                        energy = actor->members[j].maximum;
                    }
                } else if (actor->members[j].mode == 2) {
                    actor->cooldown--;
                    if (actor->cooldown == 0) {
                        actor->members[j].mode = 1;
                        energy = 40.0f;
                    }
                }
            } else {
                energy += 0.5;
                if (energy > actor->members[j].maximum) {
                    energy = actor->members[j].maximum;
                }
            }
            if (energy <= 0.0f) {
                if (j != actor->current) {
                    GF_AssertFail();
                }
                actor->members[j].energy = 0.0f;
                if (actor->members[j].mode != 2) {
                    actor->members[j].mode = 2;
                    actor->tired = TRUE;
                    actor->cooldown = actor->members[j].cooldown;
                    ov96_021E8228(course, actor->owner, actor->current, 1, 1);
                }
            } else if (energy <= 40.0) {
                actor->members[j].energy = energy;
                actor->members[j].mode = 1;
            } else {
                actor->members[j].energy = energy;
                actor->members[j].mode = 0;
            }
        }
    }
}

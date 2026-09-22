#include "global.h"

typedef struct ManagedSprite ManagedSprite;
typedef struct SysTask SysTask;

typedef struct Ov92Main {
    u8 pad00[0x14];
    void *context;
    u8 pad18[0x28];
    ManagedSprite *sprites[36];
} Ov92Main;

typedef struct Ov92Particle {
    u8 pad00[0x14];
    ManagedSprite *sprite;
    s32 baseX;
    s32 baseY;
    s32 velocityX;
    s32 velocityY;
    s32 offsetX;
    s32 offsetY;
    s32 trigX;
    s32 trigY;
    u8 type;
    u8 draw;
    u8 delay;
    u8 phase;
    s32 angle;
    s16 accel;
    s16 radiusX;
    s16 radiusY;
    s16 timer;
} Ov92Particle;

typedef struct Ov92TaskWork {
    Ov92Main *main;
    s32 state;
    s32 timer;
    s32 group;
    s32 *advance;
    u8 particles[36 * sizeof(Ov92Particle)];
} Ov92TaskWork;

typedef struct {
    s16 v[10];
} Ov92Positions;
extern const Ov92Positions ov92_02263E5C;
BOOL IsPaletteFadeFinished(void);
void SysTask_Destroy(SysTask *task);
void Heap_Free(void *ptr);
void ManagedSprite_SetPositionFxXYWithSubscreenOffset(ManagedSprite *, fx32, fx32, fx32);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_SetAnim(ManagedSprite *, int);
void PlaySE(u16);
void ov92_0225DF0C(Ov92Main *, int, int);
void ov92_0225DF28(Ov92Main *);
fx32 GF_SinDeg(u16);
fx32 GF_CosDeg(u16);
void ov92_02260AE0(SysTask *, Ov92TaskWork *);

void ov92_02260AE0(SysTask *task, Ov92TaskWork *work) {
    Ov92Positions positions;
    s32 startX;
    s32 startY;
    s32 j;
    s32 spriteOffset;
    s32 x;
    s32 y;
    int i;
    Ov92Particle *particle;
    void *context = work->main->context;

    if (!IsPaletteFadeFinished() || *((u8 *)context + 0x34) == 1) {
        *work->advance = 0;
        SysTask_Destroy(task);
        Heap_Free(work);
        return;
    }

    switch (work->state) {
    case 0:
        work->group = 0;
        work->state++;
    case 1:
        positions = ov92_02263E5C;
        i = 0;
        startX = positions.v[work->group * 2];
        startY = positions.v[work->group * 2 + 1];
        work->timer = i;
        particle = (Ov92Particle *)work;
        x = startX << 12;
        y = startY << 12;
        spriteOffset = i;
        for (; i < 36; i++, particle = (Ov92Particle *)((u8 *)particle + 0x34), spriteOffset += 4) {
            int rem;
            particle->delay = i % 2;
            rem = i % 3;
            particle->radiusX = 6 - rem;
            particle->radiusY = 6 - rem;
            particle->sprite = *(ManagedSprite **)((u8 *)work->main + 0x40 + spriteOffset);
            particle->angle = (i / 3) * 30;
            particle->angle %= 360;
            particle->accel = 0;
            particle->timer = -(rem * 2);
            {
                float f;
                if (startX) {
                    f = (float)(u32)x + 0.5f;
                } else {
                    f = (float)(u32)x - 0.5f;
                }
                particle->velocityX = (s32)f;
            }
            {
                float f;
                if (startY) {
                    f = (float)(u32)y + 0.5f;
                } else {
                    f = (float)(u32)y - 0.5f;
                }
                particle->velocityY = (s32)f;
            }
            particle->trigX = 0;
            particle->trigY = 0;
            particle->baseX = 0;
            particle->baseY = 0;
            particle->offsetX = 0;
            particle->offsetY = 0;
            particle->type = 0;
            particle->draw = 1;
            particle->phase = 0;
            if (particle->angle == 180) {
                particle->type = 2;
            }
            if (particle->angle >= 270 && particle->angle <= 90) {
                particle->type = 1;
            }
            {
                s32 radius = particle->radiusX;
                float f;
                s32 amp;
                if (radius > 0) {
                    f = (float)(radius << 12) + 0.5f;
                } else {
                    f = (float)(radius << 12) - 0.5f;
                }
                amp = (s32)f;
                particle->trigX = FX_Mul(GF_SinDeg((u16)particle->angle), amp);
            }
            {
                s32 radius = particle->radiusY;
                float f;
                s32 amp;
                if (radius > 0) {
                    f = (float)(radius << 12) + 0.5f;
                } else {
                    f = (float)(radius << 12) - 0.5f;
                }
                amp = (s32)f;
                particle->trigY = FX_Mul(GF_CosDeg((u16)particle->angle), amp);
            }
            {
                s32 accel = particle->accel;
                float f;
                if (accel > 0) {
                    f = (float)(accel << 12) + 0.5f;
                } else {
                    f = (float)(accel << 12) - 0.5f;
                }
                particle->baseY += (s32)f;
            }
            ManagedSprite_SetPositionFxXYWithSubscreenOffset(particle->sprite,
                particle->offsetX + particle->velocityX + particle->trigX + particle->baseX,
                particle->offsetY + particle->velocityY + particle->trigY + particle->baseY,
                14 << 16);
        }
        work->state++;
        break;
    case 2:
        if (work->timer == 0) {
            work->timer++;
            ov92_0225DF0C(work->main, 1, 0);
            PlaySE(0x58B);
        }
        if (work->timer > 30) {
            work->state = 1;
            work->group++;
            work->group %= 5;
            ov92_0225DF0C(work->main, 0, 1);
        } else {
            work->timer++;
            j = 0;
            particle = (Ov92Particle *)work;
            for (; j < 36; j++, particle = (Ov92Particle *)((u8 *)particle + 0x34)) {
                particle->timer++;
                if (particle->timer < 0) {
                    continue;
                }
                if (particle->timer >= particle->delay + 10) {
                    if (particle->timer == particle->delay + 10) {
                        ManagedSprite_SetAnim(particle->sprite, 0);
                    }
                    if (particle->timer == particle->delay + 18) {
                        particle->draw = 0;
                    } else if ((particle->timer % 2) != 0) {
                        particle->draw ^= 1;
                    }
                    ManagedSprite_SetDrawFlag(particle->sprite, particle->draw);
                    switch (particle->phase) {
                    case 0:
                        particle->offsetX = -4096;
                        break;
                    case 1:
                        particle->offsetX = 0;
                        break;
                    case 2:
                        particle->offsetX = 4096;
                        break;
                    }
                    particle->phase++;
                    particle->phase %= 3;
                }
                s16 radialX;
                s16 radialY;
                if (particle->timer > 5) {
                    switch (particle->type) {
                    case 2:
                        particle->accel = (particle->timer + 5) / 6;
                        break;
                    case 1:
                        particle->accel = (particle->timer + 5) / 6;
                        break;
                    case 0:
                        particle->accel = (particle->timer + 5) / 7;
                        break;
                    }
                }
                switch (particle->type) {
                case 2:
                    radialX = ((30 - (particle->timer + 5)) / 8);
                    radialY = ((30 - (particle->timer + 7)) / 8);
                    break;
                case 1:
                default:
                    radialX = ((30 - (particle->timer + 5)) / 8);
                    radialY = ((30 - (particle->timer + 6)) / 8);
                    break;
                case 0:
                    radialX = ((30 - (particle->timer + 5)) / 8);
                    radialY = ((30 - (particle->timer + 4)) / 8);
                    break;
                }
                if (radialX > 0) {
                    particle->radiusX += radialX;
                } else {
                    particle->radiusX++;
                }
                if (radialY > 0) {
                    particle->radiusY += radialY;
                } else {
                    particle->radiusY++;
                }
                {
                    s32 radius = particle->radiusX;
                    float f;
                    s32 amp;
                    if (radius > 0) {
                        f = (float)(radius << 12) + 0.5f;
                    } else {
                        f = (float)(radius << 12) - 0.5f;
                    }
                    amp = (s32)f;
                    particle->trigX = FX_Mul(GF_SinDeg((u16)particle->angle), amp);
                }
                {
                    s32 radius = particle->radiusY;
                    float f;
                    s32 amp;
                    if (radius > 0) {
                        f = (float)(radius << 12) + 0.5f;
                    } else {
                        f = (float)(radius << 12) - 0.5f;
                    }
                    amp = (s32)f;
                    particle->trigY = FX_Mul(GF_CosDeg((u16)particle->angle), amp);
                }
                {
                    s32 accel = particle->accel;
                    float f;
                    if (accel > 0) {
                        f = (float)(accel << 12) + 0.5f;
                    } else {
                        f = (float)(accel << 12) - 0.5f;
                    }
                    particle->baseY += (s32)f;
                }
                ManagedSprite_SetPositionFxXYWithSubscreenOffset(particle->sprite,
                    particle->offsetX + particle->velocityX + particle->trigX + particle->baseX,
                    particle->offsetY + particle->velocityY + particle->trigY + particle->baseY,
                    14 << 16);
            }
        }
        if (*work->advance != 0) {
            work->state++;
        }
        break;
    default:
        ov92_0225DF0C(work->main, 0, 0);
        Heap_Free(work);
        SysTask_Destroy(task);
        break;
    }
    ov92_0225DF28(work->main);
}

#include "to57_overlay91_run074_private.h"

void ov91_02260D98(Particle *particles) {
    int i;
    s32 dx;
    s32 dy;
    VecFx32 scale;

    for (i = 0; i < 8; i++) {
        Particle *particle = &particles[i];

        if (particle->active == TRUE) {
            if (particle->age < 16) {
                ov91_02260EE0(particle->age, 16, 8, 190, 125, &dx, &dy);
            } else if (particle->age < 26) {
                ov91_02260EE0(particle->age - 16, 10, 4, 182, 140, &dx, &dy);
            } else {
                ov91_02260EE0(particle->age - 26, 6, 3, 180, 160, &dx, &dy);
            }

            particle->age++;
            scale.x = FX_Div((9 - particle->age * 3 / 32) * FX32_ONE, 10 * FX32_ONE);
            scale.y = scale.x;
            Sprite_SetAffineScale(particle->sprite, &scale);

            if (particle->direction) {
                particle->position.x += dx << FX32_SHIFT;
            } else {
                particle->position.x -= dx << FX32_SHIFT;
            }
            particle->position.y += dy << FX32_SHIFT;
            Sprite_SetMatrix(particle->sprite, &particle->position);

            if (particle->position.x < -0x20000 || particle->position.x > 0x120000 || particle->position.y < 0x1E0000 || particle->position.y > 0x2E0000) {
                particle->active = FALSE;
                Sprite_SetDrawFlag(particle->sprite, FALSE);
            }
        }
    }
}

void ov91_02260E88(Particle *particles, s32 x, s32 y) {
    int i;
    Particle *particle = NULL;

    for (i = 0; i < 8; i++) {
        if (!particles[i].active) {
            particle = &particles[i];
            break;
        }
    }

    if (particle != NULL) {
        particle->direction = x < 0x80;
        particle->position.x = x << FX32_SHIFT;
        particle->position.y = (y << FX32_SHIFT) + 0x200000;
        Sprite_SetMatrix(particle->sprite, &particle->position);
        particle->active = TRUE;
        Sprite_SetDrawFlag(particle->sprite, TRUE);
        particle->age = 0;
    }
}

#pragma force_active on
#include "global.h"

typedef struct Particle {
    int state;
    VecFx32 pos;
    VecFx32 vel;
    VecFx32 accel;
    u8 pad28[0xc];
    u8 counter;
    u8 pad35[0x13];
} Particle;

typedef struct Work {
    u8 pad0[0xc4];
    Particle particles[12];
} Work;

Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
void ov96_02203468(void *course);

// @fn ov96_02203468
void ov96_02203468(void *course) {
    Work *w = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;
    BOOL reset;

    for (i = 0; i < 12; i++) {
        reset = FALSE;
        if (w->particles[i].state == 1) {
            VEC_MultAdd(FX32_ONE, &w->particles[i].vel, &w->particles[i].pos, &w->particles[i].pos);
        } else if (w->particles[i].state == 3) {
            w->particles[i].counter++;
            if (w->particles[i].counter >= 8) {
                w->particles[i].counter = 0;
                w->particles[i].state = 0;
            }
        }
        if (w->particles[i].pos.x < FX32_ONE || w->particles[i].pos.x > 64 * FX32_ONE || w->particles[i].pos.z < FX32_ONE || w->particles[i].pos.z > 64 * FX32_ONE) {
            reset = TRUE;
        }
        if (reset) {
            w->particles[i].state = 0;
            w->particles[i].vel.x = 0;
            w->particles[i].vel.z = 0;
            w->particles[i].vel.y = 0;
            w->particles[i].accel.x = 0;
            w->particles[i].accel.z = 0;
            w->particles[i].accel.y = 0;
        }
    }
}

#pragma force_active on
#include "global.h"
#include <nitro/fx/fx_vec.h>

typedef struct Input {
    int active;
    u8 x;
    u8 y;
    u8 pad6[0x1e];
} Input;

typedef struct CopyEntry {
    u8 data[0x28];
} CopyEntry;

typedef struct Particle {
    u8 pad0[8];
    int state;
    VecFx32 pos;
    VecFx32 vel;
    VecFx32 accel;
    VecFx32 touch;
    u8 pad3C[4];
    u16 count;
    u16 level;
    u8 pad44[4];
} Particle;

typedef struct Racer {
    f32 weight;
    u8 pad4[8];
    u32 group;
    u8 pad10[2];
    s16 stat;
    u8 blocked;
    u8 pad15[0xb];
} Racer;

typedef struct Touch {
    int pressed;
    int held;
    int unk8;
} Touch;

typedef struct Work {
    u8 pad0[0xBC];
    Particle particles[12];
    Racer racers[12];
    Touch touches[4];
    u8 sel[4];
    u8 finished;
    u8 pad5D1[0xB];
    void *obj;
    u8 pad5E0[8];
    u16 timer;
    u8 hold;
} Work;

extern const VecFx32 ov96_0221C98C[];

CopyEntry *PokeathlonCourse_GetDataCopyArea(void *course);
Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
int ov96_021E5F24(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_02202738(Work *w, u8 *dest);
u8 ov96_022031A8(Work *w, u8 idx, u8 x, u8 y, VecFx32 *out);
void ov96_02204320(int group, VecFx32 *vec);
void ov96_022043C0(void *obj, int value);
void ov96_022033FC(void *course);
void ov96_02203468(void *course);
void ov96_02203754(Work *w);
void ov96_02203544(void *course, Work *w);
void ov96_0220223C(void *course);

// @fn ov96_0220223C
void ov96_0220223C(void *course) {
    VecFx32 v;
    VecFx32 touch;
    VecFx32 v2;
    int i;
    Work *w;
    CopyEntry *copy;
    Input *input;
    int k;
    f32 factor;
    f32 mag;
    f32 hold;
    f32 res;

    copy = PokeathlonCourse_GetDataCopyArea(course);
    w = PokeathlonCourse_GetHeapAllocPtr4(course);
    if (ov96_021E5F24(course) != 0) {
        return;
    }
    if (w->finished) {
        copy++;
        ov96_02202738(w, ov96_021E8A20(copy));
        return;
    }
    *(Input *)ov96_021E8A20(&copy[2]) = *(Input *)ov96_021E8A20(copy);
    if (w->timer != 0) {
        w->timer--;
    }
    ov96_021E8A20(&copy[1]);
    for (i = 0; i < 4; i++) {
        input = (Input *)ov96_021E8A20(&(copy + 2)[i]);
        if (input->active) {
            if (w->touches[i].pressed && w->touches[i].held) {
                w->touches[i].pressed = 0;
            } else if (w->touches[i].pressed == 0 && w->touches[i].held == 0) {
                w->touches[i].pressed = 1;
                w->touches[i].held = 1;
            }
        } else {
            w->touches[i].pressed = 0;
            w->touches[i].held = 0;
        }
        if (w->touches[i].pressed) {
            k = ov96_022031A8(w, i, input->x, input->y, &v);
            if (k != 12) {
                if (w->racers[k].blocked != 0) {
                    continue;
                }
                if (w->particles[k].state == 0 || w->particles[k].state == 2) {
                    w->sel[i] = k;
                    w->particles[k].accel = v;
                    touch.x = input->x << 12;
                    touch.z = input->y << 12;
                    touch.y = 0;
                    w->particles[k].touch = touch;
                    if (w->particles[k].state == 0) {
                        w->particles[k].pos = ov96_0221C98C[k];
                        w->particles[k].state = 2;
                        w->particles[k].count = 0;
                        w->particles[k].level = 0;
                    }
                }
            }
            w->hold = 0;
        } else if (w->touches[i].held) {
            if (w->sel[i] != 12 && w->hold < 30.0) {
                w->hold++;
            }
        } else {
            if (w->sel[i] != 12) {
                k = ov96_022031A8(w, i, input->x, input->y, &v2);
                if (k != 12 && k == w->sel[i]) {
                    if (w->particles[k].count < 4) {
                        w->particles[k].count++;
                        if (w->particles[k].count <= 1) {
                            w->particles[k].level = 0;
                        } else if (w->particles[k].count <= 2) {
                            w->particles[k].level = 1;
                        } else if (w->particles[k].count <= 3) {
                            w->particles[k].level = 2;
                        } else {
                            w->particles[k].level = 3;
                        }
                    }
                } else {
                    VecFx32 zero = { 0, 0, 0 };
                    VecFx32 a;
                    VecFx32 c;
                    VecFx32 b;
                    int idx = w->sel[i];

                    VEC_Subtract(&v2, &w->particles[idx].accel, &a);
                    b.x = input->x << 12;
                    b.z = input->y << 12;
                    b.y = 0;
                    VEC_Subtract(&b, &w->particles[idx].touch, &c);
                    if (c.z <= 0) {
                        w->particles[idx].vel = a;
                        if (w->racers[idx].stat <= 60) {
                            factor = 0.5f;
                        } else if (w->racers[idx].stat <= 70) {
                            factor = 0.6f;
                        } else if (w->racers[idx].stat <= 80) {
                            factor = 0.7f;
                        } else if (w->racers[idx].stat <= 90) {
                            factor = 0.8f;
                        } else {
                            factor = 1.0f;
                        }
                        mag = VEC_Mag(&w->particles[idx].vel) / FX32_ONE;
                        if (mag > 2.0) {
                            mag = 2.0f;
                        } else if (mag < 1.0) {
                            mag = 1.0f;
                        }
                        hold = 1.0 + (f32)w->hold / 30.0;
                        res = factor * (mag * (2.0 - w->racers[idx].weight)) / hold;
                        if (res > 3.0) {
                            res = 3.0f;
                        } else if (res < 1.0) {
                            res = 1.0f;
                        }
                        VEC_Normalize(&w->particles[idx].vel, &w->particles[idx].vel);
                        VEC_MultAdd(4096.0f * res, &w->particles[idx].vel, &zero, &w->particles[idx].vel);
                        ov96_02204320((u8)w->racers[idx].group, &w->particles[idx].vel);
                        w->particles[idx].state = 1;
                    }
                }
                w->sel[i] = 12;
            }
            w->hold = 0;
        }
    }
    ov96_022043C0(w->obj, 1800 - w->timer);
    ov96_022033FC(course);
    ov96_02203468(course);
    ov96_02203754(w);
    ov96_02203544(course, w);
    if (w->timer == 0) {
        w->finished = 1;
    } else {
        w->finished = 0;
    }
    copy++;
    ov96_02202738(w, ov96_021E8A20(copy));
}

#pragma force_active on
#include "global.h"

typedef struct Input {
    int active;
    u8 x;
    u8 y;
    u8 pad6[2];
    u8 phase;
    u8 pad9[0x1b];
} Input;

typedef struct CopyEntry {
    u8 data[0x28];
} CopyEntry;

typedef struct Speed {
    f32 a;
    f32 b;
    u8 pad8[0xc];
} Speed;

typedef struct Player {
    u8 pad0[4];
    u8 body[0xc];
    Speed speeds[3];
    u8 pad4C[0x10];
    int posX;
    int posY;
    u8 pad64[4];
    VecFx32 vel;
    u8 pad74[0xa];
    u16 holdTime;
    VecFx32 touchStart;
    VecFx32 touchEnd;
    u8 pad98[8];
    u8 active;
    u8 stage;
    u8 padA2[6];
    u8 blocked;
    u8 release;
    u8 locked;
    u8 padAB[4];
    u8 reset;
    u8 padB0[4];
    u8 changed;
    u8 speedIdx;
    u8 padB6[2];
} Player;

typedef struct Touch {
    int pressed;
    int held;
    int unk8;
} Touch;

typedef struct Work {
    u8 pad0[0x20];
    Player players[4];
    u8 pad300[4];
    Touch touches[4];
    int dragging[4];
    u8 pad344[0x28];
    void *obj;
    u8 pad370[0x19c];
    u16 timer;
    u8 finished;
    u8 pad50F;
    u8 holdMax;
    u8 pad511[0xa];
    u8 updated;
    u8 pad51C[4];
    s16 outX;
    s16 outY;
} Work;

CopyEntry *PokeathlonCourse_GetDataCopyArea(void *course);
Work *PokeathlonCourse_GetHeapAllocPtr4(void *course);
int ov96_021E5F24(void *course);
u8 *ov96_021E8A20(void *ptr);
void ov96_02205AFC(Work *w, u8 *dest);
int PokeathlonPennantCaptureCpu_Update(void *obj);
BOOL ov96_02205DD4(Work *w, u8 idx, u8 x, u8 y);
f32 ov96_022073F0(void *p, int index);
void PokeathlonPennantCapture_AddPoint2D(VecFx32 *a, VecFx32 *b, VecFx32 *out);
int PokeathlonPennantCapture_GetDirection(VecFx32 *v);
void PokeathlonPennantCapture_UpdatePlayers(void *course);
void PokeathlonPennantCapture_RefillPennants(Work *w);
void ov96_022055AC(void *course);

static inline void ResetTouch(Player *p) {
    p->holdTime = 0;
    p->touchStart.x = 0;
    p->touchStart.y = 0;
    p->touchStart.z = 0;
    p->touchEnd = p->touchStart;
}

// @fn ov96_022055AC
void ov96_022055AC(void *course) {
    VecFx32 vel;
    VecFx32 delta;
    int i;
    Work *w;
    Work *self;
    CopyEntry *copy;
    Input *input;
    BOOL done;
    f32 a, b;
    f32 limit;
    fx32 scale;
    f32 mag;
    f32 len;

    copy = PokeathlonCourse_GetDataCopyArea(course);
    w = PokeathlonCourse_GetHeapAllocPtr4(course);
    self = w;
    if (ov96_021E5F24(course) != 0) {
        return;
    }
    if (w->finished) {
        copy++;
        ov96_02205AFC(w, ov96_021E8A20(copy));
        return;
    }
    *(Input *)ov96_021E8A20(&copy[2]) = *(Input *)ov96_021E8A20(copy);
    if (w->timer != 0) {
        w->timer--;
    }
    PokeathlonPennantCaptureCpu_Update(w->obj);
    w->updated = 0;
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
        if (w->players[i].active) {
            if (w->players[i].stage == 1 && input->phase == 1) {
                w->players[i].speedIdx = (w->players[i].speedIdx + 1) % 3;
                w->players[i].stage = 2;
                w->players[i].changed = 1;
            } else if (w->players[i].stage == 2 && input->phase == 2) {
                w->players[i].stage = 0;
                w->players[i].active = 0;
                w->players[i].reset = 0;
                w->players[i].changed = 1;
            }
            w->dragging[i] = 0;
            ResetTouch(&w->players[i]);
        } else if (w->players[i].locked == 1) {
            w->dragging[i] = 0;
            ResetTouch(&w->players[i]);
        } else if (w->players[i].blocked) {
            w->dragging[i] = 0;
            ResetTouch(&w->players[i]);
        } else if (w->players[i].reset) {
            w->dragging[i] = 0;
            ResetTouch(&w->players[i]);
        } else {
            done = FALSE;
            if (w->touches[i].pressed) {
                if (ov96_02205DD4(w, i, input->x, input->y)) {
                    w->dragging[i] = 1;
                    w->players[i].touchStart.x = input->x << 12;
                    w->players[i].touchStart.y = input->y << 12;
                    w->players[i].touchStart.z = 0;
                    w->players[i].touchEnd = w->players[i].touchStart;
                }
            } else if (w->touches[i].held) {
                if (w->players[i].holdTime < 0xff) {
                    w->players[i].holdTime++;
                } else {
                    w->holdMax = 1;
                }
                if (w->dragging[i] == 0) {
                    continue;
                }
                w->players[i].touchEnd.x = input->x << 12;
                w->players[i].touchEnd.y = input->y << 12;
            } else {
                if (w->dragging[i]) {
                    if (w->players[i].holdTime <= 30) {
                        w->players[i].release = 1;
                    }
                    w->dragging[i] = 0;
                    w->holdMax = 0;
                }
                done = TRUE;
            }
            if (w->players[i].release) {
                VecFx32 zero = { 0, 0, 0 };
                VecFx32 result;

                a = w->players[i].speeds[w->players[i].speedIdx].a;
                b = w->players[i].speeds[w->players[i].speedIdx].b;
                limit = ov96_022073F0(w->players[i].body, w->players[i].speedIdx);
                scale = a * b / 8.0 * 4096.0;
                VEC_Subtract(&w->players[i].touchEnd, &w->players[i].touchStart, &delta);
                VEC_MultAdd(scale, &delta, &zero, &delta);
                len = VEC_Mag(&delta) / FX32_ONE;
                vel = w->players[i].vel;
                PokeathlonPennantCapture_AddPoint2D(&vel, &delta, &vel);
                mag = VEC_Mag(&vel);
                if (mag > 4096.0f * limit) {
                    VecFx32 zero2 = { 0, 0, 0 };

                    VEC_Normalize(&vel, &vel);
                    VEC_MultAdd(4096.0f * limit, &vel, &zero2, &w->players[i].vel);
                    VEC_MultAdd(4096.0f * limit, &vel, &zero2, &result);
                } else {
                    PokeathlonPennantCapture_AddPoint2D(&self->players[i].vel, &delta, &w->players[i].vel);
                    result = delta;
                }
                w->players[i].changed = PokeathlonPennantCapture_GetDirection(&self->players[i].vel);
                w->updated = 1;
                w->outY = w->players[i].posX / FX32_ONE;
                w->outX = w->players[i].posY / FX32_ONE;
                w->players[i].release = 0;
            }
            if (done) {
                ResetTouch(&w->players[i]);
            }
        }
    }
    PokeathlonPennantCapture_UpdatePlayers(course);
    PokeathlonPennantCapture_RefillPennants(w);
    if (w->timer == 0) {
        w->finished = 1;
    } else {
        w->finished = 0;
    }
    copy++;
    ov96_02205AFC(w, ov96_021E8A20(copy));
}

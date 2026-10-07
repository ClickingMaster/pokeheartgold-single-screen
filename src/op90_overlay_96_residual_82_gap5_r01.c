#pragma force_active on
#include "global.h"

typedef struct Particle {
    void *objA;
    void *objB;
    void *objC;
    u8 pad0C[0x35];
    u8 frame;
    u8 pad42[6];
} Particle;

typedef struct Player {
    void *obj;
    u8 pad4[0x1c];
} Player;

typedef struct Positions {
    VecFx32 pos[3];
} Positions;

typedef struct Work {
    u8 pad0[0x44];
    void *gfx;
    u8 pad48[4];
    void *frameObj;
    void *sprites[2];
    void *icons[3];
    void *markers[3];
    void *digits[9];
    u8 pad94[0x24];
    Particle particles[12];
    Player players[12];
    u8 pad598[0x3c];
    void *manager;
} Work;

extern Positions ov96_0221C910;

void *ov96_021EB5E8(void *res);
void *ov96_021EA374(void *manager, void *res, u32 a, void *gfx);
void Sprite_SetDrawFlag(void *sprite, u32 flag);
void Sprite_SetDrawPriority(void *sprite, u32 priority);
void Sprite_SetMatrix(void *sprite, VecFx32 *pos);
void ov96_022038A0(Work *w, u32 v);
void *ov96_021EB3E4(void *res, int a, int b, int c, int d);
void ov96_021EB564(void *obj, int anim);
void ov96_021EB630(void *obj, int priority);
void ov96_021EB588(void *obj, VecFx32 *pos);
void ov96_021EB52C(void *obj, int a, int b);
void ov96_02201EF0(Work *w, void *res, int start);

// @fn ov96_02201EF0
#pragma opt_common_subs off
void ov96_02201EF0(Work *w, void *res, int start) {
    int i;
    u8 anim;
    VecFx32 pos;
    VecFx32 scale;
    VecFx32 framePos;
    Positions positions;

    for (i = 0; i < 2; i++) {
        w->sprites[i] = ov96_021EA374(w->manager, ov96_021EB5E8(res), 3, w->gfx);
        Sprite_SetDrawFlag(w->sprites[i], TRUE);
        pos.x = (i * 16) * FX32_ONE + 24 * FX32_ONE;
        pos.y = 8 * FX32_ONE;
        pos.z = 0;
        Sprite_SetMatrix(w->sprites[i], &pos);
        Sprite_SetDrawPriority(w->sprites[i], 2);
    }
    ov96_022038A0(w, 60);
    for (i = 0; i < 12; i++) {
        w->particles[i].frame = i;
        w->particles[i].objA = ov96_021EB3E4(res, 3, 3, 0x6a, 3);
        w->particles[i].objB = ov96_021EB3E4(res, 3, 3, 0x6a, 4);
        w->particles[i].objC = ov96_021EB3E4(res, 3, 3, 0x6a, 6);
        ov96_021EB564(w->particles[i].objB, 1);
        ov96_021EB630(w->particles[i].objA, 20);
        ov96_021EB630(w->particles[i].objB, 1000);
        ov96_021EB630(w->particles[i].objC, 5);
    }
    for (i = 0; i < 12; i++) {
        w->players[i].obj = ov96_021EB3E4(res, 3, 3, 0x6a, 9);
        ov96_021EB564(w->players[i].obj, 2);
        scale.x = FX32_ONE;
        scale.y = FX32_ONE;
        scale.z = 0;
        ov96_021EB588(w->players[i].obj, &scale);
        ov96_021EB630(w->players[i].obj, 4);
    }
    for (i = 0; i < 3; i++) {
        w->markers[i] = ov96_021EB3E4(res, 2, 1, 0x69, 11);
        ov96_021EB564(w->markers[i], 6);
        ov96_021EB630(w->markers[i], 2);
    }
    w->frameObj = ov96_021EB3E4(res, 3, 1, 0x69, 7);
    framePos.x = 8 * FX32_ONE;
    framePos.y = 8 * FX32_ONE;
    framePos.z = 0;
    ov96_021EB588(w->frameObj, &framePos);
    ov96_021EB564(w->frameObj, 5);
    ov96_021EB52C(w->frameObj, 1, 1);
    ov96_021EB630(w->frameObj, 2);
    positions = ov96_0221C910;
    anim = (start + 1) % 4;
    for (i = 0; i < 3; i++) {
        w->icons[i] = ov96_021EB3E4(res, 3, 1, 0x69, 8);
        ov96_021EB588(w->icons[i], &positions.pos[i]);
        ov96_021EB564(w->icons[i], anim + 1);
        ov96_021EB52C(w->icons[i], 1, 1);
        anim = (anim + 1) % 4;
        ov96_021EB630(w->icons[i], 3);
    }
    for (i = 0; i < 9; i++) {
        w->digits[i] = ov96_021EB3E4(res, 3, 1, 0x69, 10);
        ov96_021EB564(w->digits[i], 0);
        ov96_021EB630(w->digits[i], 4);
    }
}
#pragma opt_common_subs reset

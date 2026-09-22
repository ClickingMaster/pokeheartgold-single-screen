#ifndef TO57_NEW10_PRIVATE_H
#define TO57_NEW10_PRIVATE_H
#include "global.h"

#include "math_util.h"
typedef struct Ov112Particle {
    u8 active;
    u8 pad1;
    s8 speedFactor;
    s8 velocity;
    s16 x;
    s16 y;
    s16 dx;
    s16 angleSin;
    u16 angle;
    s16 life;
} Ov112Particle;
typedef struct Ov112State {
    u8 pad[0x1E530];
    void *sprites[468];
    Ov112Particle particles[100];
} Ov112State;
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
void ManagedSprite_SetPositionXY(void *, int, int);
void ManagedSprite_SetAnim(void *, int);
void ManagedSprite_SetDrawFlag(void *, int);
void ManagedSprite_SetDrawPriority(void *, int);
int ov112_021EA9A0(void *);
void ov112_021EA6D8(Ov112State *, int, int, int, int);
void ov112_021EA76C(Ov112State *);
void ov112_021EA7D0(Ov112State *);
int ov112_021EA838(Ov112State *);
void ov112_021EA864(Ov112State *);
void ov112_021EA984(void *);
#endif

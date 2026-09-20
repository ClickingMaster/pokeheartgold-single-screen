#ifndef POKEHEARTGOLD_TO56_W01_PRIVATE_H
#define POKEHEARTGOLD_TO56_W01_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "pokepic.h"

typedef struct To56W01Motion {
    s16 x;
    s16 y;
    u8 pad04[0x20];
} To56W01Motion;

typedef struct To56W01Work {
    void *owner;
    int state;
    int counter;
    Pokepic *pic;
    s16 x;
    s16 y;
    s16 start;
    s16 end;
    void *handle;
    To56W01Motion movement;
    To56W01Motion motion;
} To56W01Work;

u32 *ov07_02222D60(void *handle);
void ov07_0222C850(To56W01Work *work, u32 *table);
BOOL ov07_022222B4(To56W01Motion *motion);
u32 ov07_02222D88(u16 x, u16 y);
void ov07_02231E74(void *, int, int);
void ov07_02222AC4(void *, int, int, int, int, int);
BOOL ov07_02222AF4(void *);
void ov07_02222D74(void *);
void ov07_02222D3C(void *);
BgConfig *ov07_0221C4A0(void *);
int ov07_0221FAEC(void *, int);
int ov07_0221FB04(void *, int);
int ov07_0221FAE8(void *);
int ov07_0221BFD0(void *);
void ov07_0221C448(void *, int);
BOOL ov07_0222C870(To56W01Work *work);
void ov07_0222C918(int task, To56W01Work *work);

#endif

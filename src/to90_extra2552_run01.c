#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Oam {
    u32 attr01;
    u16 attr2;
    u16 affine;
} Oam;

typedef struct Node {
    struct Node *prev, *next;
} Node;

typedef struct OamBlk {
    Node node;
    Oam *oam;
    u8 count;
} OamBlk;

typedef struct Work {
    void *resources[4];
    Oam *objects[10], *extras[2];
    OamBlk *sprites[2];
    void *data[4], *one, *effect;
    u8 flags[12];
} Work;

extern Work *ov13_0224F4C0;
extern const u8 ov13_0224521C[4];
extern const u8 ov13_02245210[4];
void WifiSetupOamBlock_SetModeAndPalette(OamBlk *, int, u32, u32);
void ov13_0222AD5C(int, int);
typedef char CheckOam[(sizeof(Oam) == 8) ? 1 : -1];
typedef char CheckWork[(sizeof(Work) == 108) ? 1 : -1];

static inline void SetPalette(Oam *object, u32 palette) {
    object->attr01 &= ~0xc00u;
    object->attr2 = (object->attr2 & ~0xf000u) | (palette << 12);
}

void ov13_0222AD5C(int index, int mode) {
    if (index < 0) {
        return;
    }
    if (index < 10) {
        Work *w = ov13_0224F4C0;
        SetPalette(w->objects[index], ov13_0224521C[mode]);
    } else if (index - 10 < 2) {
        Work *w = ov13_0224F4C0;
        SetPalette(w->extras[index - 10], ov13_0224521C[mode]);
    } else {
        Work *w = ov13_0224F4C0;
        WifiSetupOamBlock_SetModeAndPalette(w->sprites[index - 12], -1, 0, ov13_02245210[mode]);
    }
}

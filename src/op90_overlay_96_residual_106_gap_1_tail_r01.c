#pragma force_active on
#include "global.h"
#include "sprite_system.h"

typedef struct Work {
    u8 pad0[8];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 pad10[0x58];
    ManagedSprite *sprites[31];
} Work;

ManagedSprite *ov96_022104D8(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 animation, u16 priority);
void ov96_0221075C(Work *w);

static inline int GetSign(int col) {
    if (col == 0) {
        return 1;
    }
    return -1;
}

// @fn ov96_0221075C
void ov96_0221075C(Work *w) {
    int row, col;
    int y, idx;

    s16 x;
    int slot;

    for (row = 0, idx = 7, y = 8; row < 12; idx += 2, y += 16, row++) {
        for (col = 0; col < 2; col++) {
            slot = col + idx;
            x = 128;
            x += GetSign(col) * ((row + 4) * 8);
            w->sprites[slot] = ov96_022104D8(w->spriteSystem, w->spriteManager, x, y, 20, 64);
            ManagedSprite_SetPriority(w->sprites[slot], 3);
        }
    }
}

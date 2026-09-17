#include "global.h"

#include "pokedex.h"
#include "sprite_system.h"

typedef struct IconEntry {
    u16 species;
    u16 flag;
} IconEntry;

typedef struct IconGridWork {
    Pokedex **pokedex;
    u8 pad004[0x66C];
    ManagedSprite *sprites[624];
    IconEntry entries[522];
    u8 unk1858;
    u8 page;
    u8 pad185A[4];
    u8 row;
} IconGridWork;

void ov18_021F1598(void *, int, int);
void ov18_021F1620(IconGridWork *, int);

void ov18_021F1620(IconGridWork *p, int base) {
    u16 i;
    u16 spriteIdx;
    int col;
    int row;

    for (i = 0; i < 30; i++) {
        spriteIdx = base + i + 30 * (p->row ^ 1);
        ManagedSprite_SetDrawFlag(p->sprites[spriteIdx], FALSE);
    }
    for (i = 0; i < 30; i++) {
        spriteIdx = base + i + 30 * p->row;
        ov18_021F1598(p, i + 15 * p->page, spriteIdx);
        col = i % 5;
        row = i / 5;
        ManagedSprite_SetPositionXY(p->sprites[spriteIdx], 0x30 + 0x28 * col, 0x18 + 0x28 * row);
    }
}

#ifndef TO52_OV57_R7_PRIVATE_H
#define TO52_OV57_R7_PRIVATE_H
#include "global.h"
#include "filesystem.h"
#include "palette.h"
#include "pokemon.h"
#include "seal_case.h"
#include "sprite_system.h"

typedef struct Ov57SpriteSlot {
    int anim;
    ManagedSprite *sprite;
    int unk8;
} Ov57SpriteSlot;

typedef struct Ov57R7Work {
    Pokemon **mons;
    u32 monCapsule[12];
    CAPSULE *capsules[12];
    u8 pad_064[0x78];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    u8 pad_0E4[4];
    PaletteData *paletteData;
    u8 pad_0EC[0x1A4];
    Ov57SpriteSlot slots[12];
    ManagedSprite *extra1;
    ManagedSprite *extra2;
} Ov57R7Work;

void ov57_022392F4(Ov57R7Work *, NARC *);
void ov57_022394AC(Ov57R7Work *);
void ov57_022394D8(Ov57R7Work *, int, int);
void ov57_02238DAC(int, s16 *, s16 *);
void ov57_02238F48(Ov57R7Work *);
void ov57_02239014(Ov57R7Work *);
#endif

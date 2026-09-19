#ifndef W18_R6_PRIVATE_H
#define W18_R6_PRIVATE_H
#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "party_menu.h"
#include "sprite_system.h"

typedef struct W18FrontierMap {
    void *bgConfig;
    PaletteData *paletteData;
    u8 pad08[0x2C];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
} W18FrontierMap;

extern const ManagedSpriteTemplate ov80_0223BD4C;
void ov80_0222F33C(W18FrontierMap *);
void ov80_0222F3CC(W18FrontierMap *);
ManagedSprite *ov80_0222F3FC(W18FrontierMap *, s16, s16);

#endif

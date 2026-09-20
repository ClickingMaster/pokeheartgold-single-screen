#ifndef TO55_R3_W07_PRIVATE_H
#define TO55_R3_W07_PRIVATE_H

#include "global.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "sprite_system.h"

typedef struct W07Work {
    u8 pad00000[0x1E528];
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *sprites[217];
} W07Work;

extern const OamCharTransferParam ov112_021FEC98;
extern const SpriteResourceCountsListUnion ov112_021FECAC;
extern const OamManagerParam ov112_021FECC4;
extern const u8 ov112_021FEC88[];
extern const ManagedSpriteTemplate ov112_021FED1C[];
extern const ManagedSpriteTemplate ov112_021FF028;
extern const ManagedSpriteTemplate ov112_021FF05C;

void GF_CreateVramTransferManager(u32, enum HeapID);
void GfGfx_EngineBTogglePlanes(u8, u8);
void G2dRenderer_SetSubSurfaceCoords(G2dRenderer *, fx32, fx32);
void ov112_021EAB58(void);
void ov112_021EA984(void *);
void ov112_021EA1AC(W07Work *);
void ov112_021EA1F8(W07Work *);
void ov112_021EA230(W07Work *);
void ov112_021EA2C4(W07Work *);
void ov112_021EA3C0(W07Work *);

#endif

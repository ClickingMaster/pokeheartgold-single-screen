#ifndef POKEHEARTGOLD_BILLBOARD_GFX_SEQUENCE_H
#define POKEHEARTGOLD_BILLBOARD_GFX_SEQUENCE_H

#include "global.h"

typedef struct BillboardTexPlttIndex {
    u8 textureIdx;
    u8 plttIdx;
} BillboardTexPlttIndex;

// Which texture and palette of a billboard's texture file each frame range shows.
typedef struct BillboardGfxSequence {
    const u16 *startFrame;
    const u8 *textureIdx;
    const u8 *plttIdx;
    u32 seqCount;
} BillboardGfxSequence;

void BillboardGfxSequence_Init(const void *data, BillboardGfxSequence *gfxSequence);
BillboardTexPlttIndex BillboardGfxSequence_GetTexPlttIndex(const BillboardGfxSequence *, u32);

#endif // POKEHEARTGOLD_BILLBOARD_GFX_SEQUENCE_H

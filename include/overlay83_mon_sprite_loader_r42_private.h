#ifndef POKEHEARTGOLD_OVERLAY83_MON_SPRITE_LOADER_R42_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY83_MON_SPRITE_LOADER_R42_PRIVATE_H
#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite.h"
#include "unk_02013FDC.h"
typedef struct Ov83SpriteSlot {
    void *unk0;
    Sprite *sprite;
} Ov83SpriteSlot;
extern const UnkStruct_02014E30 ov83_0224818C;
void ov83_022475D4(void *, int);
void ov83_02247568(void *, void *, u32);
void ov83_0224755C(void *, int);
void PlaySE(u16);
void DC_FlushRange(const void *, u32);
void GXS_LoadOBJ(const void *, u32, u32);
void ov83_02247630(void *, void *, u32);
void ov83_02247668(Ov83SpriteSlot *, BoxPokemon *, u16, u32);
#endif

#ifndef TO52_R5_W09_PRIVATE_H
#define TO52_R5_W09_PRIVATE_H

#include "global.h"

#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_loader.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite.h"

typedef struct Ov71WorkR46 {
    void *ctx;
    u8 pad04[0x10 - 0x04];
    PokepicManager *pokepicManager;
    Pokepic *pokepic;
    PokepicAnimScript animScript[10];
    Sprite *sprite0;
    Sprite *sprite1;
    u8 resources[0x3c];
    NARC *narc;
} Ov71WorkR46;

BoxPokemon *TradeSequence_GetReceivedBoxMon(void *ctx);
u16 TradeSequence_GetReceivedSpecies(void *ctx);
void TradeSequence_LoadCellAnim(void *dst, s32 narcId, s32 a2, s32 a3);
void TradeSequence_InitSpriteHeader(void *dst, void *src, NNSG2dImageProxy *image, NNSG2dImagePaletteProxy *palette, u8 a4);
Sprite *TradeSequence_CreateSprite(void *ctx, void *config, s32 x, s32 y, s32 a4, s32 a5);
void ov71_0224B720(Ov71WorkR46 *work);
Pokepic *ov71_0224B7EC(Ov71WorkR46 *work);
void ov71_0224B848(Ov71WorkR46 *work);

#endif
